#include "controller.hpp"
#include <iostream>
struct SimOutputs : unlock::Outputs {
  void camera(bool on) override { std::cout << "SIM camera=" << on << '\n'; }
  void light(bool on) override { std::cout << "SIM light=" << on << '\n'; }
};
int main() {
  SimOutputs outputs;
  unlock::Controller c(outputs);
  c.enable(); c.proximity(0);
  c.evidence(1000, {c.attempt(), true, true});
  if (c.outcome() != unlock::Outcome::DryRun) return 1;
  std::cout << "SIM recognition passed -> DRY_RUN only; no vehicle command sent\n";
  c.emergency();
}
