#include "FreeInfo.h"
#include "InfoGetters.hpp"
#include "argparse/include/argparse/argparse.hpp"
#include "InfoStrParser.hpp"

#include <sstream>
#include <optional>
#include <expected>
#include <list>
#include <regex>
#include <unordered_map>
#include <functional>
#include <stdlib.h>

const char *INFO_ERROR_NAMES[] = {
    "No",
    "Kernel Version",
    "Linux Name",
    "Time String",
    "Script Error",
    "Parse Error",
    "Feature Error",
    "Multiple Errors"
};
[[maybe_unused]] const size_t INFO_ERROR_NAMES_LEN = ARRAY_SIZE(INFO_ERROR_NAMES);

//TODO: We need to clean up how multiple cmd line args are handled
//TODO: Workout why the command parser adds an extra \ before the \n chars
//This issue is a property of the terminal

void printErrors(InfoError errors) {
  if(errors & SCRIPT_ERROR) {
    printf("You need to spcify a script to append the output of\n");
    return;
  }

  printf("The following errors occored:\n");
  InfoError tmpBitPos = FIRST_ERROR;
  for(InfoError i = 0; i < (int)INFO_ERROR_NUM_BITS; i++){
    InfoError sepparatedError = tmpBitPos & errors;
    if(sepparatedError)
      //first string is for no errors so +1
      printf("%s\n", INFO_ERROR_NAMES[i+1]);
    tmpBitPos <<= 1;
  }
}

class InfoGetter {
private:
    std::string osRelFile;
    std::string luaScriptFile;
    std::unordered_map<std::string, std::function<char*()>> infoFlagFuncMap;
    std::list<char*> generatedStringsToDel;
    InfoError error;

    char *handleLuaScript() {
      //free ignores null pointers so no need to set a default value
      char *ret = nullptr;
      if (luaScriptFile.empty()) {
        error |= SCRIPT_ERROR;
        return ret;
      }

      ret = getLuaScriptString(this->luaScriptFile.c_str());
      return ret;
    }

    void infoFuncMapSetup(){
      infoFlagFuncMap["%KVR"] = getKernelVer;
      infoFlagFuncMap["%LVR"] = [this](){return getLinNmFrom(this->osRelFile.c_str());};
      infoFlagFuncMap["%TIM"] = getTimeStr;
      infoFlagFuncMap["%SCP"] = [this](){return handleLuaScript();};
    }

public:
    InfoGetter(): osRelFile(), luaScriptFile(), infoFlagFuncMap(), generatedStringsToDel(), error{} {
        infoFuncMapSetup();
    }

    InfoGetter(std::string osRLF): osRelFile(osRLF), luaScriptFile(), infoFlagFuncMap(), generatedStringsToDel(), error{} {
        infoFuncMapSetup();
    }

    InfoGetter(std::string osRLF, std::string luaScript): osRelFile(osRLF), luaScriptFile(luaScript), infoFlagFuncMap(), generatedStringsToDel(), error{} {
        infoFuncMapSetup();
    }

    ~InfoGetter(){
      for(auto i = generatedStringsToDel.begin(); i != generatedStringsToDel.end(); i++){
        //Thease strings where allocated with malloc so best to use free
        //free ignores null pointers so no need to check
        free(*i);
      }
    }

    InfoError getError() {
      return error;
    }

    void setOSReleaseFile(std::string osRLF){
      this->osRelFile = osRLF;
    }

    void setScript(std::string luaScript) {
      this->luaScriptFile = luaScript;
    }

    bool hasFunc(std::string funcFlag){
      if(infoFlagFuncMap[funcFlag])
        return true;
      return false;
    }

    std::optional<std::string> getStringFromMatch(std::string in){
      std::optional<std::string> ret = std::nullopt;
      if(hasFunc(in)){
        auto tmp = infoFlagFuncMap[in];
        char *resCStr = tmp();
        //Some functions return null when they fail
        if (resCStr)
          ret = std::optional(std::string(resCStr));
        if(ret.has_value())
          generatedStringsToDel.push_front(resCStr);
      }

      return ret;
    }

};

InfoError insertFeatureData(std::string& output, std::string& feature, InfoGetter& ig) {
  InfoError ret = NO_ERROR;
  if(ig.hasFunc(feature)) {
    auto potentialFeatureData = ig.getStringFromMatch(feature);
    if (potentialFeatureData.has_value())
      output += potentialFeatureData.value();
    else
      //FIXME: we should eventurally have a code for each function
      ret |= FEATURE_ERROR | ig.getError();
  } else {
    ret = PARSE_ERROR;
  }

  return ret;
}

//This should be integrated into the parse layout function so we dont loop through the data twice
std::string insertNewLineChars(std::string input) {
  std::string output = "";
  for(int i = 0; i < (int)input.size(); i++){
    bool newLine = i > 0 && input[i-1] == '\\' && input[i] == 'n';
    bool backslash = input[i] == '\\';
    if(newLine) {
      output.push_back('\n');
    } else if (!backslash) {
      output.push_back(input[i]);
    }
  }
  return output;
}

std::expected<std::string, InfoError> parseLayoutString(std::string lString, InfoGetter& ig) {
  //TODO: figure out why we couldn't make a regex match "\\%"
  std::regex optMatcher("%[a-zA-Z]{3}");
  std::smatch match;
  auto workStr = lString;
  std::string output = "";
  auto error = 0;

  while(std::regex_search(workStr, match, optMatcher)){
    output += insertNewLineChars(workStr.substr(0, match.position(0)));
    auto matchStr = match[0].str();
    std::transform(matchStr.begin(), matchStr.end(), matchStr.begin(), [](unsigned char c){return std::toupper(c);});
    //TODO: when we add the feature to ignore "\\%" add logic here to not set that to insertFeat...
    error += insertFeatureData(output, matchStr, ig);
    workStr = match.suffix();
  }
  output += workStr.substr(0, match.position(0));

  if(error)
    return std::unexpected((InfoError)error);
  return output;
}

int main(int argc, char **argv) {
  InfoError bad = NO_ERROR;
  auto args = argparse::parse<Args>(argc, argv);
  InfoGetter ig(args.verinfo);

  if (!args.script.empty())
    ig.setScript(args.script);

  std::string defaultInfo("Running kernel %KVR on %LVR\n%TIM\n\n");
  std::string input = defaultInfo;

  if(!args.layout.empty()) {
    input = args.layout;
    input = insertNewLineChars(input);
  }

  auto outputOr = parseLayoutString(input, ig);
  if(outputOr.has_value()){
    puts(outputOr->c_str());
  } else {
    bad |= outputOr.error();
  }
  if(bad)
    printErrors(bad);

  return 0;
}
