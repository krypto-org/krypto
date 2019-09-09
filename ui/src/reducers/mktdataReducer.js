import {
  REDUX_WEBSOCKET_CLOSED,
  REDUX_WEBSOCKET_CONNECT,
  REDUX_WEBSOCKET_DISCONNECT,
  REDUX_WEBSOCKET_MESSAGE,
  REDUX_WEBSOCKET_OPEN,
  REDUX_WEBSOCKET_SEND,
  GENERATE_PRICE_CACHE
} from '../actions/mktdataActions';


const initialState = {
  prices: [],
  trades: [],
  tableMap: {},
  initialized: false,
  connected: false
};

export default function mktdataReducer(state = initialState, action) {
  console.log(action)
  switch (action.type) {
    case GENERATE_PRICE_CACHE:
      return {
        ...state,
        prices: action.payload.prices,
        tableMap: action.payload.tableMap,
        initialized: true
      };
    case REDUX_WEBSOCKET_CONNECT:
      return state
    case REDUX_WEBSOCKET_DISCONNECT:
      return {
        ...state,
        connected: false,
      };
    case REDUX_WEBSOCKET_OPEN:
      return {
        ...state,
        connected: true,
      };
    case REDUX_WEBSOCKET_CLOSED:
      return {
        ...state,
        connected: false,
      };
    case REDUX_WEBSOCKET_MESSAGE:
      {
        let data = JSON.parse(action.payload.message)
        if (data.kind == "quote") {
          let idx = state.tableMap[data.security_id]
          const prices = state.prices.map((price, index) => {
            if (idx !== index) {
              return price
            }

            return {
              ...price,
              ...data,
            }
          })
          let newState = {
            ...state,
            prices: prices
          }
          return newState
        }
        return state
      }
    case REDUX_WEBSOCKET_SEND:
      return state
    default:
      return state
  }
}
