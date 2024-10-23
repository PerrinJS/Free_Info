#include "FreeInfo.h"
#include "InfoGetters.hpp"
#include "argparse/include/argparse/argparse.hpp"
#include "InfoStrParser.hpp"

#include <sstream>
#include <optional>
#include <list>
#include <regex>
#include <unordered_map>
#include <functional>

class InfoGetter {
private:
    std::string osRelFile;
    std::unordered_map<std::string, std::function<char*()>> infoFlagFuncMap;
    std::list<char*> generatedStringsToDel;

    void infoFuncMapSetup(){
      infoFlagFuncMap["%KVR"] = getKernelVer;
      infoFlagFuncMap["%LVR"] = [this](){return getLinNmFrom(this->osRelFile.c_str());};
      infoFlagFuncMap["%TIM"] = getTimeStr;
    }

public:
    InfoGetter(): osRelFile(), infoFlagFuncMap(), generatedStringsToDel(){
        infoFuncMapSetup();
    }

    InfoGetter(std::string osRLF): osRelFile(osRLF), infoFlagFuncMap(), generatedStringsToDel(){
        infoFuncMapSetup();
    }

    ~InfoGetter(){
      for(auto i = generatedStringsToDel.begin(); i != generatedStringsToDel.end(); i++){
        //Thease strings where allocated with malloc so best to use free
        if (*i != nullptr)
          free(*i);
      }
    }

    void setOSReleaseFile(std::string osRLF){
      this->osRelFile = osRLF;
    }

    std::optional<std::string> getStringFromMatch(std::string in){
      std::optional<std::string> ret = std::nullopt;
      if(auto tmp = infoFlagFuncMap[in]){
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

int insertFeatureData(std::string& output, std::string& feature, InfoGetter& ig) {
  auto potentialFeatureData = ig.getStringFromMatch(feature);
  auto ret = 0;
  if (potentialFeatureData.has_value())
    output += potentialFeatureData.value();
  else
    ret = 1;

  return ret;
}

std::optional<std::string> parseLayoutString(std::string lString, InfoGetter& ig) {
  //TODO: figure out why we can't also match "\\%"
  std::regex optMatcher("%[a-zA-Z]{3}");
  std::smatch match;
  auto workStr = lString;
  std::string output = "";
  auto error = 0;

  while(std::regex_search(workStr, match, optMatcher)){
    output += workStr.substr(0, match.position(0));
    auto matchStr = match[0].str();
    //TODO: when we add the feature to ignore "\\%" add logic here to not set that to insertFeat...
    error += insertFeatureData(output, matchStr, ig);
    workStr = match.suffix();
  }
  output += workStr.substr(0, match.position(0));

  auto ret = error ? std::nullopt : std::optional(output);
  return ret;
}

int main(int argc, char **argv) {
  int bad = 0;
  auto args = argparse::parse<Args>(argc, argv);
  InfoGetter ig(args.verinfo);

  std::string defaultInfo("Running kernel %KVR on %LVR\n%TIM\n");
  std::string input = defaultInfo;

  if(args.layout.length() != 0) {
    input = args.layout;
  }

  auto outputOr = parseLayoutString(input, ig);
  if(outputOr.has_value()){
    puts(outputOr->c_str());
  } else {
    bad += PARSE_ERROR;
  }
  if(bad)
    //TODO: print a more usefull error string
    printf("Oops something went wrong\n");

  return 0;
}
