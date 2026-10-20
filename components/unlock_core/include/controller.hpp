#pragma once
#include <cstdint>
namespace unlock {
enum class State { Disabled, Idle, Recognizing, Cooldown, Emergency };
enum class Outcome { None, Denied, TimedOut, DryRun, Unavailable };
struct Evidence { uint32_t attempt; bool matched; bool live; };
// Board implementations must fail closed on output/power errors.
struct Outputs {
  virtual ~Outputs() = default;
  virtual void camera(bool on) = 0;
  virtual void light(bool on) = 0;
};
// First release deliberately has no live command transport.
class Controller {
 public:
  explicit Controller(Outputs& outputs) : outputs_(outputs) { off(); }
  void enable() { if (state_ == State::Disabled) state_ = State::Idle; }
  void disable() { off(); if (state_ != State::Emergency) state_ = State::Disabled; }
  void emergency() { off(); state_ = State::Emergency; }
  // Emergency latch can only be cleared into Disabled by explicit local action.
  void reset() { off(); state_ = State::Disabled; }
  void proximity(uint64_t now) {
    tick(now);
    if (state_ != State::Idle) return;
    ++attempt_; started_ = now; outcome_ = Outcome::None;
    state_ = State::Recognizing; outputs_.camera(true); outputs_.light(true);
  }
  void evidence(uint64_t now, Evidence e) {
    tick(now);
    if (state_ != State::Recognizing || e.attempt != attempt_) return;
    outcome_ = e.matched && e.live ? Outcome::DryRun : Outcome::Denied;
    finish(now);
  }
  void tick(uint64_t now) {
    if (state_ == State::Recognizing && now - started_ >= 8000) {
      outcome_ = Outcome::TimedOut; finish(now);
    } else if (state_ == State::Cooldown && now - started_ >= 15000) {
      state_ = State::Idle;
    }
  }
  State state() const { return state_; }
  Outcome outcome() const { return outcome_; }
  uint32_t attempt() const { return attempt_; }
 private:
  void off() { outputs_.light(false); outputs_.camera(false); }
  void finish(uint64_t now) { off(); started_ = now; state_ = State::Cooldown; }
  Outputs& outputs_;
  State state_ = State::Disabled;
  Outcome outcome_ = Outcome::None;
  uint64_t started_ = 0;
  uint32_t attempt_ = 0;
};
}
