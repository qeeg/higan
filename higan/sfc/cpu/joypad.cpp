//called every 256 clocks; see CPU::addClocks()
auto CPU::stepAutoJoypadPoll() -> void {
  if(vcounter() >= ppu.vdisp()) {
    //cache enable state at first iteration
    if(status.autoJoypadCounter == 0) status.autoJoypadLatch = status.autoJoypadPoll;
    status.autoJoypadActive = status.autoJoypadCounter <= 15;

    if(status.autoJoypadActive && status.autoJoypadLatch) {
      if(status.autoJoypadCounter == 0) {
        SuperFamicom::peripherals.controllerPort1->latch(1);
        SuperFamicom::peripherals.controllerPort2->latch(1);
        SuperFamicom::peripherals.controllerPort1->latch(0);
        SuperFamicom::peripherals.controllerPort2->latch(0);
      }

      uint2 port0 = SuperFamicom::peripherals.controllerPort1->data();
      uint2 port1 = SuperFamicom::peripherals.controllerPort2->data();

      status.joy1 = status.joy1 << 1 | port0.bit(0);
      status.joy2 = status.joy2 << 1 | port1.bit(0);
      status.joy3 = status.joy3 << 1 | port0.bit(1);
      status.joy4 = status.joy4 << 1 | port1.bit(1);
    }

    status.autoJoypadCounter++;
  }
}