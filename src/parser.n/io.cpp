#include "parser.n/io.h"
#include "utils/codecs.hpp"
#include "utils/strutils.hpp"
#include "utils/logging.hpp"

namespace ltp {
namespace depparser {

using strutils::chomp;
using strutils::split;
using strutils::to_int;
using strutils::to_str;

CoNLLReader::CoNLLReader(std::istream& _is,
    bool _trace): trace(_trace), LineCountsReader(_is) {
}

Instance* CoNLLReader::next() {
  if (is.eof()) { return NULL;}

  Instance* inst = new Instance;
  std::string line;

  inst->forms.push_back( SpecialOption::ROOT );
  inst->lemmas.push_back( SpecialOption::ROOT );
  inst->postags.push_back( SpecialOption::ROOT );
  inst->heads.push_back( -1 );
  inst->deprels.push_back( SpecialOption::ROOT );

  while (!is.eof()) {
    getline(is, line);
    chomp(line);

    if (line.size() == 0) { break; }
    std::vector<std::string> items = split(line);

    if (items.size() < 8) {
      WARNING_LOG("Unknown conll format file");
    }

    inst->forms.push_back( items[1] );    // items[1]: form
    inst->lemmas.push_back( items[2] );   // items[2]: lemma
    inst->postags.push_back( items[3] );  // items[4]: postag
    inst->heads.push_back( to_int(items[6]) );
    inst->deprels.push_back( items[7] );
  }

  if (inst->forms.size() == 1) {
    delete inst;
    inst = NULL;
  }

  return inst;
}

void CoNLLWriter::write(const Instance& inst) {
  size_t len = inst.size();
  bool predicted = (inst.predict_heads.size() == len &&
      inst.predict_deprels.size() == len);

  for (size_t i = 1; i < len; ++ i) {
    f << i << "\t"                  // 0 - index
      << inst.forms[i]   << "\t"   // 1 - form
      << inst.lemmas[i]  << "\t"   // 2 - lemma
      << inst.postags[i] << "\t"   // 3 - postag
      << "_"              << "\t"   // 4 - unknown
      << "_"              << "\t"   // 5 - unknown
      << inst.heads[i]   << "\t"   // 6 - heads
      << inst.deprels[i] << "\t"   // 7 - deprels
      << (predicted ? to_str(inst.predict_heads[i]) : "_")
      << "\t"
      << (predicted ? inst.predict_deprels[i] : "_")
      << std::endl;
  }
  f << std::endl;
}

} //  namespace depparser
} //  namespace ltp