// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_EVENTS_H_
#define FLATBUFFERS_GENERATED_EVENTS_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 2 &&
              FLATBUFFERS_VERSION_MINOR == 0 &&
              FLATBUFFERS_VERSION_REVISION == 6,
             "Non-compatible flatbuffers version included");

struct Event;

struct EventPacket;
struct EventPacketBuilder;

FLATBUFFERS_MANUALLY_ALIGNED_STRUCT(8) Event FLATBUFFERS_FINAL_CLASS {
 private:
  int64_t t_;
  int16_t x_;
  int16_t y_;
  uint8_t on_;
  int8_t padding0__;  int16_t padding1__;

 public:
  Event()
      : t_(0),
        x_(0),
        y_(0),
        on_(0),
        padding0__(0),
        padding1__(0) {
    (void)padding0__;
    (void)padding1__;
  }
  Event(int64_t _t, int16_t _x, int16_t _y, bool _on)
      : t_(flatbuffers::EndianScalar(_t)),
        x_(flatbuffers::EndianScalar(_x)),
        y_(flatbuffers::EndianScalar(_y)),
        on_(flatbuffers::EndianScalar(static_cast<uint8_t>(_on))),
        padding0__(0),
        padding1__(0) {
    (void)padding0__;
    (void)padding1__;
  }
  int64_t t() const {
    return flatbuffers::EndianScalar(t_);
  }
  int16_t x() const {
    return flatbuffers::EndianScalar(x_);
  }
  int16_t y() const {
    return flatbuffers::EndianScalar(y_);
  }
  bool on() const {
    return flatbuffers::EndianScalar(on_) != 0;
  }
};
FLATBUFFERS_STRUCT_END(Event, 16);

struct EventPacket FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef EventPacketBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ELEMENTS = 4
  };
  const flatbuffers::Vector<const Event *> *elements() const {
    return GetPointer<const flatbuffers::Vector<const Event *> *>(VT_ELEMENTS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ELEMENTS) &&
           verifier.VerifyVector(elements()) &&
           verifier.EndTable();
  }
};

struct EventPacketBuilder {
  typedef EventPacket Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_elements(flatbuffers::Offset<flatbuffers::Vector<const Event *>> elements) {
    fbb_.AddOffset(EventPacket::VT_ELEMENTS, elements);
  }
  explicit EventPacketBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<EventPacket> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<EventPacket>(end);
    return o;
  }
};

inline flatbuffers::Offset<EventPacket> CreateEventPacket(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::Vector<const Event *>> elements = 0) {
  EventPacketBuilder builder_(_fbb);
  builder_.add_elements(elements);
  return builder_.Finish();
}

inline flatbuffers::Offset<EventPacket> CreateEventPacketDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const std::vector<Event> *elements = nullptr) {
  auto elements__ = elements ? _fbb.CreateVectorOfStructs<Event>(*elements) : 0;
  return CreateEventPacket(
      _fbb,
      elements__);
}

inline const EventPacket *GetEventPacket(const void *buf) {
  return flatbuffers::GetRoot<EventPacket>(buf);
}

inline const EventPacket *GetSizePrefixedEventPacket(const void *buf) {
  return flatbuffers::GetSizePrefixedRoot<EventPacket>(buf);
}

inline bool VerifyEventPacketBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifyBuffer<EventPacket>(nullptr);
}

inline bool VerifySizePrefixedEventPacketBuffer(
    flatbuffers::Verifier &verifier) {
  return verifier.VerifySizePrefixedBuffer<EventPacket>(nullptr);
}

inline void FinishEventPacketBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<EventPacket> root) {
  fbb.Finish(root);
}

inline void FinishSizePrefixedEventPacketBuffer(
    flatbuffers::FlatBufferBuilder &fbb,
    flatbuffers::Offset<EventPacket> root) {
  fbb.FinishSizePrefixed(root);
}

#endif  // FLATBUFFERS_GENERATED_EVENTS_H_