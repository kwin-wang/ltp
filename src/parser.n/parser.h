#ifndef __LTP_PARSERN_PARSER_H__
#define __LTP_PARSERN_PARSER_H__

#include "utils/smartmap.hpp"
#include "utils/unordered_map.hpp"
#include "parser.n/system.h"
#include "parser.n/context.h"
#include "parser.n/classifier.h"

namespace ltp {
namespace depparser {

class NeuralNetworkParser {
protected:
  utility::IndexableSmartMap forms_alphabet;
  utility::IndexableSmartMap postags_alphabet;
  utility::IndexableSmartMap deprels_alphabet;
  utility::IndexableSmartMap cluster4_types_alphabet;
  utility::IndexableSmartMap cluster6_types_alphabet;
  utility::IndexableSmartMap cluster_types_alphabet;

  std::unordered_map<int, int> precomputation_id_encoder;
  std::unordered_map<int, int> form_to_cluster4;
  std::unordered_map<int, int> form_to_cluster6;
  std::unordered_map<int, int> form_to_cluster;

  NeuralNetworkClassifier classifier;
  TransitionSystem system;

  size_t kNilForm;
  size_t kNilPostag;
  size_t kNilDeprel;
  size_t kNilDistance;
  size_t kNilValency;
  size_t kNilCluster4;
  size_t kNilCluster6;
  size_t kNilCluster;

  size_t kFormInFeaturespace;
  size_t kPostagInFeaturespace;
  size_t kDeprelInFeaturespace;
  size_t kDistanceInFeaturespace;
  size_t kValencyInFeaturespace;
  size_t kCluster4InFeaturespace;
  size_t kCluster6InFeaturespace;
  size_t kClusterInFeaturespace;
  size_t kFeatureSpaceEnd;

  size_t nr_feature_types;

  bool use_distance;
  bool use_valency;
  bool use_cluster;

public:
  NeuralNetworkParser();

  void get_features(const State& state,
      std::vector<int>& features);

  void get_features(const State& state,
      const std::vector<int>& cluster4,
      const std::vector<int>& cluster6,
      const std::vector<int>& cluster,
      std::vector<int>& features);

  void load(const std::string& filename);

  void save(const std::string& filename);

protected:
  void get_context(const State& state, Context* ctx);

  void get_basic_features(const Context& ctx,
      const std::vector<int>& forms,
      const std::vector<int>& postags,
      const std::vector<int>& deprels,
      std::vector<int>& features);

  void get_distance_features(const Context& ctx,
      std::vector<int>& features);

  void get_valency_features(const Context& ctx,
      const std::vector<int>& nr_left_children,
      const std::vector<int>& nr_right_children,
      std::vector<int>& features);

  void get_cluster_features(const Context& ctx,
      const std::vector<int>& cluster4,
      const std::vector<int>& cluster6,
      const std::vector<int>& cluster,
      std::vector<int>& features);

  void report();

  void transduce_instance_to_dependency(const Instance& data,
      Dependency* dependency, bool with_dependencies);

  void get_cluster_from_dependency(const Dependency& dependency,
      std::vector<int>& cluster4,
      std::vector<int>& cluster6,
      std::vector<int>& cluster);
};

} //  namespace depparser
} //  namespace ltp

#endif  //  end for __LTP_PARSERN_PARSER_H__