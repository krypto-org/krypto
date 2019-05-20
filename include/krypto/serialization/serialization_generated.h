// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_SERIALIZATION_KRYPTO_SERIALIZATION_H_
#define FLATBUFFERS_GENERATED_SERIALIZATION_KRYPTO_SERIALIZATION_H_

#include "flatbuffers/flatbuffers.h"

namespace krypto {
namespace serialization {

struct SequenceNumber;

struct Quote;

struct SnapshotPriceLevel;

struct Snapshot;

struct IncrementalPriceLevel;

struct Incremental;

struct Trade;

enum Side {
  Side_UNKNOWN = 0,
  Side_BUY = 1,
  Side_SELL = 2,
  Side_MIN = Side_UNKNOWN,
  Side_MAX = Side_SELL
};

inline const Side (&EnumValuesSide())[3] {
  static const Side values[] = {
    Side_UNKNOWN,
    Side_BUY,
    Side_SELL
  };
  return values;
}

inline const char * const *EnumNamesSide() {
  static const char * const names[] = {
    "UNKNOWN",
    "BUY",
    "SELL",
    nullptr
  };
  return names;
}

inline const char *EnumNameSide(Side e) {
  const size_t index = static_cast<int>(e);
  return EnumNamesSide()[index];
}

enum OrderSide {
  OrderSide_UNKNOWN = 0,
  OrderSide_BID = 1,
  OrderSide_ASK = 2,
  OrderSide_MIN = OrderSide_UNKNOWN,
  OrderSide_MAX = OrderSide_ASK
};

inline const OrderSide (&EnumValuesOrderSide())[3] {
  static const OrderSide values[] = {
    OrderSide_UNKNOWN,
    OrderSide_BID,
    OrderSide_ASK
  };
  return values;
}

inline const char * const *EnumNamesOrderSide() {
  static const char * const names[] = {
    "UNKNOWN",
    "BID",
    "ASK",
    nullptr
  };
  return names;
}

inline const char *EnumNameOrderSide(OrderSide e) {
  const size_t index = static_cast<int>(e);
  return EnumNamesOrderSide()[index];
}

MANUALLY_ALIGNED_STRUCT(8) SnapshotPriceLevel FLATBUFFERS_FINAL_CLASS {
 private:
  int64_t price_;
  int64_t quantity_;

 public:
  SnapshotPriceLevel() {
    memset(this, 0, sizeof(SnapshotPriceLevel));
  }
  SnapshotPriceLevel(int64_t _price, int64_t _quantity)
      : price_(flatbuffers::EndianScalar(_price)),
        quantity_(flatbuffers::EndianScalar(_quantity)) {
  }
  int64_t price() const {
    return flatbuffers::EndianScalar(price_);
  }
  int64_t quantity() const {
    return flatbuffers::EndianScalar(quantity_);
  }
};
STRUCT_END(SnapshotPriceLevel, 16);

MANUALLY_ALIGNED_STRUCT(8) IncrementalPriceLevel FLATBUFFERS_FINAL_CLASS {
 private:
  int64_t price_;
  int64_t quantity_;
  int64_t side_;

 public:
  IncrementalPriceLevel() {
    memset(this, 0, sizeof(IncrementalPriceLevel));
  }
  IncrementalPriceLevel(int64_t _price, int64_t _quantity, int64_t _side)
      : price_(flatbuffers::EndianScalar(_price)),
        quantity_(flatbuffers::EndianScalar(_quantity)),
        side_(flatbuffers::EndianScalar(_side)) {
  }
  int64_t price() const {
    return flatbuffers::EndianScalar(price_);
  }
  int64_t quantity() const {
    return flatbuffers::EndianScalar(quantity_);
  }
  int64_t side() const {
    return flatbuffers::EndianScalar(side_);
  }
};
STRUCT_END(IncrementalPriceLevel, 24);

struct SequenceNumber FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_VALUE = 4
  };
  int64_t value() const {
    return GetField<int64_t>(VT_VALUE, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int64_t>(verifier, VT_VALUE) &&
           verifier.EndTable();
  }
};

struct SequenceNumberBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_value(int64_t value) {
    fbb_.AddElement<int64_t>(SequenceNumber::VT_VALUE, value, 0);
  }
  explicit SequenceNumberBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  SequenceNumberBuilder &operator=(const SequenceNumberBuilder &);
  flatbuffers::Offset<SequenceNumber> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<SequenceNumber>(end);
    return o;
  }
};

inline flatbuffers::Offset<SequenceNumber> CreateSequenceNumber(
    flatbuffers::FlatBufferBuilder &_fbb,
    int64_t value = 0) {
  SequenceNumberBuilder builder_(_fbb);
  builder_.add_value(value);
  return builder_.Finish();
}

struct Quote FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_TIMESTAMP = 4,
    VT_SECURITY_ID = 6,
    VT_BID = 8,
    VT_ASK = 10,
    VT_LAST = 12,
    VT_BID_QUANTITY = 14,
    VT_ASK_QUANTITY = 16,
    VT_LAST_QUANTITY = 18
  };
  int64_t timestamp() const {
    return GetField<int64_t>(VT_TIMESTAMP, 0);
  }
  int64_t security_id() const {
    return GetField<int64_t>(VT_SECURITY_ID, 0);
  }
  int64_t bid() const {
    return GetField<int64_t>(VT_BID, 0);
  }
  int64_t ask() const {
    return GetField<int64_t>(VT_ASK, 0);
  }
  int64_t last() const {
    return GetField<int64_t>(VT_LAST, 0);
  }
  int64_t bid_quantity() const {
    return GetField<int64_t>(VT_BID_QUANTITY, 0);
  }
  int64_t ask_quantity() const {
    return GetField<int64_t>(VT_ASK_QUANTITY, 0);
  }
  int64_t last_quantity() const {
    return GetField<int64_t>(VT_LAST_QUANTITY, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int64_t>(verifier, VT_TIMESTAMP) &&
           VerifyField<int64_t>(verifier, VT_SECURITY_ID) &&
           VerifyField<int64_t>(verifier, VT_BID) &&
           VerifyField<int64_t>(verifier, VT_ASK) &&
           VerifyField<int64_t>(verifier, VT_LAST) &&
           VerifyField<int64_t>(verifier, VT_BID_QUANTITY) &&
           VerifyField<int64_t>(verifier, VT_ASK_QUANTITY) &&
           VerifyField<int64_t>(verifier, VT_LAST_QUANTITY) &&
           verifier.EndTable();
  }
};

struct QuoteBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_timestamp(int64_t timestamp) {
    fbb_.AddElement<int64_t>(Quote::VT_TIMESTAMP, timestamp, 0);
  }
  void add_security_id(int64_t security_id) {
    fbb_.AddElement<int64_t>(Quote::VT_SECURITY_ID, security_id, 0);
  }
  void add_bid(int64_t bid) {
    fbb_.AddElement<int64_t>(Quote::VT_BID, bid, 0);
  }
  void add_ask(int64_t ask) {
    fbb_.AddElement<int64_t>(Quote::VT_ASK, ask, 0);
  }
  void add_last(int64_t last) {
    fbb_.AddElement<int64_t>(Quote::VT_LAST, last, 0);
  }
  void add_bid_quantity(int64_t bid_quantity) {
    fbb_.AddElement<int64_t>(Quote::VT_BID_QUANTITY, bid_quantity, 0);
  }
  void add_ask_quantity(int64_t ask_quantity) {
    fbb_.AddElement<int64_t>(Quote::VT_ASK_QUANTITY, ask_quantity, 0);
  }
  void add_last_quantity(int64_t last_quantity) {
    fbb_.AddElement<int64_t>(Quote::VT_LAST_QUANTITY, last_quantity, 0);
  }
  explicit QuoteBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  QuoteBuilder &operator=(const QuoteBuilder &);
  flatbuffers::Offset<Quote> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Quote>(end);
    return o;
  }
};

inline flatbuffers::Offset<Quote> CreateQuote(
    flatbuffers::FlatBufferBuilder &_fbb,
    int64_t timestamp = 0,
    int64_t security_id = 0,
    int64_t bid = 0,
    int64_t ask = 0,
    int64_t last = 0,
    int64_t bid_quantity = 0,
    int64_t ask_quantity = 0,
    int64_t last_quantity = 0) {
  QuoteBuilder builder_(_fbb);
  builder_.add_last_quantity(last_quantity);
  builder_.add_ask_quantity(ask_quantity);
  builder_.add_bid_quantity(bid_quantity);
  builder_.add_last(last);
  builder_.add_ask(ask);
  builder_.add_bid(bid);
  builder_.add_security_id(security_id);
  builder_.add_timestamp(timestamp);
  return builder_.Finish();
}

struct Snapshot FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_TIMESTAMP = 4,
    VT_SECURITY_ID = 6,
    VT_BIDS = 8,
    VT_ASKS = 10
  };
  int64_t timestamp() const {
    return GetField<int64_t>(VT_TIMESTAMP, 0);
  }
  int64_t security_id() const {
    return GetField<int64_t>(VT_SECURITY_ID, 0);
  }
  const flatbuffers::Vector<const SnapshotPriceLevel *> *bids() const {
    return GetPointer<const flatbuffers::Vector<const SnapshotPriceLevel *> *>(VT_BIDS);
  }
  const flatbuffers::Vector<const SnapshotPriceLevel *> *asks() const {
    return GetPointer<const flatbuffers::Vector<const SnapshotPriceLevel *> *>(VT_ASKS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int64_t>(verifier, VT_TIMESTAMP) &&
           VerifyField<int64_t>(verifier, VT_SECURITY_ID) &&
           VerifyOffset(verifier, VT_BIDS) &&
           verifier.Verify(bids()) &&
           VerifyOffset(verifier, VT_ASKS) &&
           verifier.Verify(asks()) &&
           verifier.EndTable();
  }
};

struct SnapshotBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_timestamp(int64_t timestamp) {
    fbb_.AddElement<int64_t>(Snapshot::VT_TIMESTAMP, timestamp, 0);
  }
  void add_security_id(int64_t security_id) {
    fbb_.AddElement<int64_t>(Snapshot::VT_SECURITY_ID, security_id, 0);
  }
  void add_bids(flatbuffers::Offset<flatbuffers::Vector<const SnapshotPriceLevel *>> bids) {
    fbb_.AddOffset(Snapshot::VT_BIDS, bids);
  }
  void add_asks(flatbuffers::Offset<flatbuffers::Vector<const SnapshotPriceLevel *>> asks) {
    fbb_.AddOffset(Snapshot::VT_ASKS, asks);
  }
  explicit SnapshotBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  SnapshotBuilder &operator=(const SnapshotBuilder &);
  flatbuffers::Offset<Snapshot> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Snapshot>(end);
    return o;
  }
};

inline flatbuffers::Offset<Snapshot> CreateSnapshot(
    flatbuffers::FlatBufferBuilder &_fbb,
    int64_t timestamp = 0,
    int64_t security_id = 0,
    flatbuffers::Offset<flatbuffers::Vector<const SnapshotPriceLevel *>> bids = 0,
    flatbuffers::Offset<flatbuffers::Vector<const SnapshotPriceLevel *>> asks = 0) {
  SnapshotBuilder builder_(_fbb);
  builder_.add_security_id(security_id);
  builder_.add_timestamp(timestamp);
  builder_.add_asks(asks);
  builder_.add_bids(bids);
  return builder_.Finish();
}

inline flatbuffers::Offset<Snapshot> CreateSnapshotDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int64_t timestamp = 0,
    int64_t security_id = 0,
    const std::vector<SnapshotPriceLevel> *bids = nullptr,
    const std::vector<SnapshotPriceLevel> *asks = nullptr) {
  return krypto::serialization::CreateSnapshot(
      _fbb,
      timestamp,
      security_id,
      bids ? _fbb.CreateVectorOfStructs<SnapshotPriceLevel>(*bids) : 0,
      asks ? _fbb.CreateVectorOfStructs<SnapshotPriceLevel>(*asks) : 0);
}

struct Incremental FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_TIMESTAMP = 4,
    VT_SECURITY_ID = 6,
    VT_UPDATES = 8
  };
  int64_t timestamp() const {
    return GetField<int64_t>(VT_TIMESTAMP, 0);
  }
  int64_t security_id() const {
    return GetField<int64_t>(VT_SECURITY_ID, 0);
  }
  const flatbuffers::Vector<const IncrementalPriceLevel *> *updates() const {
    return GetPointer<const flatbuffers::Vector<const IncrementalPriceLevel *> *>(VT_UPDATES);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int64_t>(verifier, VT_TIMESTAMP) &&
           VerifyField<int64_t>(verifier, VT_SECURITY_ID) &&
           VerifyOffset(verifier, VT_UPDATES) &&
           verifier.Verify(updates()) &&
           verifier.EndTable();
  }
};

struct IncrementalBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_timestamp(int64_t timestamp) {
    fbb_.AddElement<int64_t>(Incremental::VT_TIMESTAMP, timestamp, 0);
  }
  void add_security_id(int64_t security_id) {
    fbb_.AddElement<int64_t>(Incremental::VT_SECURITY_ID, security_id, 0);
  }
  void add_updates(flatbuffers::Offset<flatbuffers::Vector<const IncrementalPriceLevel *>> updates) {
    fbb_.AddOffset(Incremental::VT_UPDATES, updates);
  }
  explicit IncrementalBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  IncrementalBuilder &operator=(const IncrementalBuilder &);
  flatbuffers::Offset<Incremental> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Incremental>(end);
    return o;
  }
};

inline flatbuffers::Offset<Incremental> CreateIncremental(
    flatbuffers::FlatBufferBuilder &_fbb,
    int64_t timestamp = 0,
    int64_t security_id = 0,
    flatbuffers::Offset<flatbuffers::Vector<const IncrementalPriceLevel *>> updates = 0) {
  IncrementalBuilder builder_(_fbb);
  builder_.add_security_id(security_id);
  builder_.add_timestamp(timestamp);
  builder_.add_updates(updates);
  return builder_.Finish();
}

inline flatbuffers::Offset<Incremental> CreateIncrementalDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int64_t timestamp = 0,
    int64_t security_id = 0,
    const std::vector<IncrementalPriceLevel> *updates = nullptr) {
  return krypto::serialization::CreateIncremental(
      _fbb,
      timestamp,
      security_id,
      updates ? _fbb.CreateVectorOfStructs<IncrementalPriceLevel>(*updates) : 0);
}

struct Trade FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_TIMESTAMP = 4,
    VT_SECURITY_ID = 6,
    VT_PRICE = 8,
    VT_QUANTITY = 10,
    VT_SIDE = 12,
    VT_ORDER_ID = 14
  };
  int64_t timestamp() const {
    return GetField<int64_t>(VT_TIMESTAMP, 0);
  }
  int64_t security_id() const {
    return GetField<int64_t>(VT_SECURITY_ID, 0);
  }
  int64_t price() const {
    return GetField<int64_t>(VT_PRICE, 0);
  }
  int64_t quantity() const {
    return GetField<int64_t>(VT_QUANTITY, 0);
  }
  Side side() const {
    return static_cast<Side>(GetField<int8_t>(VT_SIDE, 0));
  }
  const flatbuffers::String *order_id() const {
    return GetPointer<const flatbuffers::String *>(VT_ORDER_ID);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int64_t>(verifier, VT_TIMESTAMP) &&
           VerifyField<int64_t>(verifier, VT_SECURITY_ID) &&
           VerifyField<int64_t>(verifier, VT_PRICE) &&
           VerifyField<int64_t>(verifier, VT_QUANTITY) &&
           VerifyField<int8_t>(verifier, VT_SIDE) &&
           VerifyOffset(verifier, VT_ORDER_ID) &&
           verifier.Verify(order_id()) &&
           verifier.EndTable();
  }
};

struct TradeBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_timestamp(int64_t timestamp) {
    fbb_.AddElement<int64_t>(Trade::VT_TIMESTAMP, timestamp, 0);
  }
  void add_security_id(int64_t security_id) {
    fbb_.AddElement<int64_t>(Trade::VT_SECURITY_ID, security_id, 0);
  }
  void add_price(int64_t price) {
    fbb_.AddElement<int64_t>(Trade::VT_PRICE, price, 0);
  }
  void add_quantity(int64_t quantity) {
    fbb_.AddElement<int64_t>(Trade::VT_QUANTITY, quantity, 0);
  }
  void add_side(Side side) {
    fbb_.AddElement<int8_t>(Trade::VT_SIDE, static_cast<int8_t>(side), 0);
  }
  void add_order_id(flatbuffers::Offset<flatbuffers::String> order_id) {
    fbb_.AddOffset(Trade::VT_ORDER_ID, order_id);
  }
  explicit TradeBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  TradeBuilder &operator=(const TradeBuilder &);
  flatbuffers::Offset<Trade> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<Trade>(end);
    return o;
  }
};

inline flatbuffers::Offset<Trade> CreateTrade(
    flatbuffers::FlatBufferBuilder &_fbb,
    int64_t timestamp = 0,
    int64_t security_id = 0,
    int64_t price = 0,
    int64_t quantity = 0,
    Side side = Side_UNKNOWN,
    flatbuffers::Offset<flatbuffers::String> order_id = 0) {
  TradeBuilder builder_(_fbb);
  builder_.add_quantity(quantity);
  builder_.add_price(price);
  builder_.add_security_id(security_id);
  builder_.add_timestamp(timestamp);
  builder_.add_order_id(order_id);
  builder_.add_side(side);
  return builder_.Finish();
}

inline flatbuffers::Offset<Trade> CreateTradeDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    int64_t timestamp = 0,
    int64_t security_id = 0,
    int64_t price = 0,
    int64_t quantity = 0,
    Side side = Side_UNKNOWN,
    const char *order_id = nullptr) {
  return krypto::serialization::CreateTrade(
      _fbb,
      timestamp,
      security_id,
      price,
      quantity,
      side,
      order_id ? _fbb.CreateString(order_id) : 0);
}

}  // namespace serialization
}  // namespace krypto

#endif  // FLATBUFFERS_GENERATED_SERIALIZATION_KRYPTO_SERIALIZATION_H_
