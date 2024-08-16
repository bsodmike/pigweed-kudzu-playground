namespace kudzu::blinky {

class BlinkyService final
    : public ::blinky::pw_rpc::nanopb::Blinky::Service<BlinkyService> {
 public:
  void Init(pw::async2::Dispatcher& dispatcher,
            pw::Allocator& allocator,
            MonochromeLed& monochrome_led,
            PolychromeLed& polychrome_led);

  pw::Status ToggleLed(const pw_protobuf_Empty&, pw_protobuf_Empty&);

  pw::Status SetLed(const blinky_SetLedRequest& request, pw_protobuf_Empty&);

  pw::Status Blink(const blinky_BlinkRequest& request, pw_protobuf_Empty&);

  pw::Status Pulse(const blinky_CycleRequest& request, pw_protobuf_Empty&);

  pw::Status SetRgb(const blinky_RgbRequest& request, pw_protobuf_Empty&);

  pw::Status Rainbow(const blinky_CycleRequest& request, pw_protobuf_Empty&);

  pw::Status IsIdle(const pw_protobuf_Empty&,
                    blinky_BlinkIdleResponse& response);

 private:
  Blinky blinky_;
};

}  // namespace sense
