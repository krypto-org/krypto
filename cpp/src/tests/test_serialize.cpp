#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>

#include <krypto/serialization/helper.h>
#include <krypto/serialization/serialization_generated.h>
#include <krypto/utils/date_time.h>
#include <krypto/logger.h>


namespace ks = krypto::serialization;

TEST_CASE("Order Update", "[Order Update Serialization]") {
    auto timestamp = krypto::utils::current_time_in_nanoseconds();
    const krypto::utils::OrderUpdate order_update{timestamp, "ajdhalkjadsfkjdf", "fwkljhfkjhewfj",
                                                  krypto::serialization::OrderStatus_FILLED, 10};
    flatbuffers::FlatBufferBuilder fb_builder;
    krypto::serialization::serialize(fb_builder, order_update);
    auto order_update_serialized = flatbuffers::GetRoot<krypto::serialization::OrderUpdate>(
            fb_builder.GetBufferPointer());
    REQUIRE(order_update_serialized->timestamp() == timestamp);
    REQUIRE(order_update_serialized->filled_quantity() == 10);
    REQUIRE(order_update_serialized->order_id()->str() == order_update.order_id);
    REQUIRE(order_update_serialized->exchange_order_id()->str() == order_update.exchange_order_id);
    REQUIRE(order_update_serialized->status() == krypto::serialization::OrderStatus_FILLED);
}

TEST_CASE("No Positions", "[Risk Summary Serialization]") {
    auto timestamp = krypto::utils::current_time_in_nanoseconds();
    krypto::utils::RiskSummary risk_summary{timestamp, {}, {}, {}, 10000.0};
    flatbuffers::FlatBufferBuilder fb_builder;
    krypto::serialization::serialize(fb_builder, risk_summary);
    auto risk_summary_serialized = flatbuffers::GetRoot<krypto::serialization::RiskSummary>(
            fb_builder.GetBufferPointer());
    REQUIRE(risk_summary_serialized->timestamp() == timestamp);
    REQUIRE(risk_summary_serialized->pnl() == 10000.0);
    REQUIRE(risk_summary_serialized->positions()->size() == 0);
    REQUIRE(risk_summary_serialized->trader_positions()->size() == 0);
    REQUIRE(risk_summary_serialized->book_positions()->size() == 0);
}

TEST_CASE("Positions", "[Risk Summary Serialization]") {
    auto timestamp = krypto::utils::current_time_in_nanoseconds();
    krypto::utils::RiskSummary risk_summary{timestamp, {}, {}, {}, 10000.0};
    const int64_t security_id = 1234;
    const std::string book = "MAIN";
    const std::string trader = "XYZ";
    risk_summary.position_by_security_id[security_id] = {timestamp, security_id, 10.0, 10000.0, 99.0};
    risk_summary.book_positions[book] = risk_summary.position_by_security_id;
    risk_summary.trader_positions[trader] = risk_summary.position_by_security_id;

    flatbuffers::FlatBufferBuilder fb_builder;
    krypto::serialization::serialize(fb_builder, risk_summary);

    const auto risk_summary_serialized = flatbuffers::GetRoot<krypto::serialization::RiskSummary>(
            fb_builder.GetBufferPointer());
    REQUIRE(risk_summary_serialized->timestamp() == timestamp);
    REQUIRE(risk_summary_serialized->pnl() == 10000.0);

    REQUIRE(risk_summary_serialized->positions()->size() == 1);
    {
        const auto position_iterator = risk_summary_serialized->positions()->cbegin();
        REQUIRE(position_iterator->security_id() == security_id);
        REQUIRE(position_iterator->timestamp() == timestamp);
        REQUIRE(position_iterator->pnl() == 10000.0);
        REQUIRE(position_iterator->position() == 10.0);
        REQUIRE(position_iterator->mark_price() == 99.0);
    }

    REQUIRE(risk_summary_serialized->trader_positions()->size() == 1);
    const auto trader_position_iterator = risk_summary_serialized->trader_positions()->cbegin();
    REQUIRE(trader_position_iterator->trader()->str() == trader);

    {
        const auto position_iterator = trader_position_iterator->positions()->cbegin();

        REQUIRE(position_iterator->security_id() == security_id);
        REQUIRE(position_iterator->timestamp() == timestamp);
        REQUIRE(position_iterator->pnl() == 10000.0);
        REQUIRE(position_iterator->position() == 10.0);
        REQUIRE(position_iterator->mark_price() == 99.0);
    }

    REQUIRE(risk_summary_serialized->book_positions()->size() == 1);
    const auto book_position_iterator = risk_summary_serialized->book_positions()->cbegin();
    REQUIRE(book_position_iterator->book()->str() == book);

    {
        const auto position_iterator = book_position_iterator->positions()->cbegin();

        REQUIRE(position_iterator->security_id() == security_id);
        REQUIRE(position_iterator->timestamp() == timestamp);
        REQUIRE(position_iterator->pnl() == 10000.0);
        REQUIRE(position_iterator->position() == 10.0);
        REQUIRE(position_iterator->mark_price() == 99.0);
    }
}