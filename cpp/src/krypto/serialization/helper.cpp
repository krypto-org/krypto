#include <krypto/serialization/helper.h>

namespace krypto::serialization {
    void serialize(flatbuffers::FlatBufferBuilder &buffer_builder,
                   const krypto::utils::OrderUpdate &order_update) {
        auto oid_offset = buffer_builder.CreateString(order_update.order_id);
        auto ex_oid_offset = buffer_builder.CreateString(order_update.exchange_order_id);
        krypto::serialization::OrderUpdateBuilder ou_builder{buffer_builder};
        ou_builder.add_timestamp(order_update.timestamp);
        ou_builder.add_filled_quantity(order_update.filled_quantity);
        ou_builder.add_status(order_update.status);
        ou_builder.add_order_id(oid_offset);
        ou_builder.add_exchange_order_id(ex_oid_offset);
        auto ou_offset = ou_builder.Finish();
        buffer_builder.Finish(ou_offset);
    }

    void
    serialize(flatbuffers::FlatBufferBuilder &buffer_builder, const krypto::utils::RiskSummary &risk_summary) {
        std::vector<krypto::serialization::Position> serialized_positions;
        std::transform(risk_summary.position_by_security_id.cbegin(),
                       risk_summary.position_by_security_id.cend(), std::back_inserter(serialized_positions),
                       [](auto &&p) {
                           return krypto::serialization::Position{p.second.timestamp, p.second.security_id,
                                                                  p.second.position, p.second.pnl,
                                                                  p.second.mark_price};
                       });
        auto positions_offset = buffer_builder.CreateVectorOfStructs(serialized_positions);

        std::vector<flatbuffers::Offset < krypto::serialization::TraderPosition>>
        serialized_trader_positions;
        for (auto&&[trader, positions]: risk_summary.trader_positions) {
            std::vector<krypto::serialization::Position> ps;
            std::transform(positions.cbegin(), positions.cend(), std::back_inserter(ps), [](auto &&p) {
                return krypto::serialization::Position{p.second.timestamp, p.second.security_id, p.second.position,
                                                       p.second.pnl, p.second.mark_price};
            });
            serialized_trader_positions.push_back(
                    krypto::serialization::CreateTraderPositionDirect(buffer_builder, trader.c_str(), &ps));
        }
        auto trader_positions_offset = buffer_builder.CreateVector(serialized_trader_positions);

        std::vector<flatbuffers::Offset < krypto::serialization::BookPosition>>
        serialized_book_positions;
        for (auto&&[book, positions]: risk_summary.book_positions) {
            std::vector<krypto::serialization::Position> ps;
            std::transform(positions.cbegin(), positions.cend(), std::back_inserter(ps), [](auto &&p) {
                return krypto::serialization::Position{p.second.timestamp, p.second.security_id, p.second.position,
                                                       p.second.pnl, p.second.mark_price};
            });
            serialized_book_positions.push_back(
                    krypto::serialization::CreateBookPositionDirect(buffer_builder, book.c_str(), &ps));
        }
        auto book_positions_offset = buffer_builder.CreateVector(serialized_book_positions);

        krypto::serialization::RiskSummaryBuilder rs_builder{buffer_builder};
        rs_builder.add_timestamp(risk_summary.timestamp);
        rs_builder.add_pnl(risk_summary.pnl);
        rs_builder.add_positions(positions_offset);
        rs_builder.add_trader_positions(trader_positions_offset);
        rs_builder.add_book_positions(book_positions_offset);
        auto rs_offset = rs_builder.Finish();
        buffer_builder.Finish(rs_offset);
    }
}
