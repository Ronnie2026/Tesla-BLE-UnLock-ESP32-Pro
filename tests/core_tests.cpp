#include "controller.hpp"
#include <cstdlib>
#include <iostream>
#define CHECK(x) do { if (!(x)) { std::cerr << "failed line " << __LINE__ << ": " #x "\n"; std::exit(1); } } while (0)
struct MockOutputs : unlock::Outputs {
  bool cam = false, lamp = false;
  void camera(bool on) override { cam = on; }
  void light(bool on) override { lamp = on; }
};
int main() {
  using namespace unlock;
  MockOutputs o; Controller c(o);
  c.proximity(0); CHECK(c.state() == State::Disabled && !o.cam && !o.lamp);
  c.enable(); c.proximity(100); CHECK(o.cam && o.lamp);
  const auto id = c.attempt();
  c.proximity(200); CHECK(c.attempt() == id);
  c.evidence(300, {id + 1, true, true}); CHECK(c.state() == State::Recognizing);
  c.evidence(400, {id, true, false}); CHECK(c.outcome() == Outcome::Denied && !o.cam && !o.lamp);
  c.proximity(500); CHECK(c.state() == State::Cooldown);
  c.tick(15400); CHECK(c.state() == State::Idle);
  c.proximity(15500); c.evidence(23500, {c.attempt(), true, true});
  CHECK(c.outcome() == Outcome::TimedOut && !o.cam && !o.lamp);
  c.tick(38500); c.proximity(38600);
  c.evidence(38700, {id, true, true}); CHECK(c.state() == State::Recognizing);
  c.evidence(38800, {c.attempt(), true, true}); CHECK(c.outcome() == Outcome::DryRun && !o.cam);
  c.tick(53800); c.proximity(53900); c.emergency();
  CHECK(!o.cam && !o.lamp && c.state() == State::Emergency);
  c.disable(); CHECK(c.state() == State::Emergency);
  c.enable(); c.proximity(54000); CHECK(c.state() == State::Emergency);
  c.reset(); CHECK(c.state() == State::Disabled);
  c.enable(); c.proximity(55000); c.disable(); CHECK(!o.cam && !o.lamp);
  std::cout << "PASS: disabled, timeout boundary, replay, liveness, cooldown, dry-run, emergency\n";
}
