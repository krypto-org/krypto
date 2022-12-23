#include <krypto/utils/types.h>

#include <utility>
#include <krypto/utils/date_time.h>

namespace krypto::utils {
    const std::unordered_map<std::string, krypto::utils::MsgType> MessageTypeData::msg_type_reference{
            name_to_msg_type()};

    Order::Order(std::string clOrderId, int64_t securityId, const std::string &symbol, double_t price,
                 double_t size, serialization::Side side, serialization::TimeInForce tif, const std::string &trader,
                 const std::string &book) :
            cl_order_id(std::move(clOrderId)),
            security_id(securityId),
            symbol(symbol),
            price(price),
            size(size),
            side(side),
            tif(tif),
            trader(trader),
            book(book),
            last_update_timestamp{krypto::utils::current_time_in_nanoseconds()},
            status{krypto::serialization::OrderStatus::OrderStatus_UNKNOWN},
            filled_quantity{std::numeric_limits<double_t>::quiet_NaN()},
            fees{0} {}

    std::ostream &operator<<(std::ostream &os, const Order &order) {
        os << "cl_order_id: " << order.cl_order_id << " security_id: " << order.security_id << " symbol: "
           << order.symbol << " price: " << order.price << " size: " << order.size << " side: " << order.side
           << " tif: " << order.tif << " order_id: " << order.order_id << " last_update_timestamp: "
           << order.last_update_timestamp << " status: " << order.status << " filled_quantity: "
           << order.filled_quantity << " fees: " << order.fees << " trader: " << order.trader << " book: "
           << order.book;
        return os;
    }

    Fill::Fill(int64_t securityId, const std::string &orderId, serialization::Side side, double_t price,
               double_t quantity, double_t remainingQuantity, const std::string &trader, const std::string &book)
            : security_id(securityId), order_id(orderId), side(side), price(price), quantity(quantity),
              remaining_quantity(remainingQuantity), trader(trader), book(book) {}

    std::ostream &operator<<(std::ostream &os, const Fill &fill) {
        os << "security_id: " << fill.security_id << " order_id: " << fill.order_id << " side: " << fill.side
           << " price: " << fill.price << " quantity: " << fill.quantity << " remaining_quantity: "
           << fill.remaining_quantity << " trader: " << fill.trader << " book: " << fill.book;
        return os;
    }
}
