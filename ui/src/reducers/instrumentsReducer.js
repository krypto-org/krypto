import {
  FETCH_INSTRUMENTS_BEGIN,
  FETCH_INSTRUMENTS_SUCCESS,
  FETCH_INSTRUMENTS_FAILURE
} from '../actions/instrumentActions';

const initialState = {
  instruments: [{
    id: "179212",
    type: "CRYPTO",
    symbol: "BTC-USD",
    exchange: "COINBASE",
    exchangeSymbol: "BTC_USD",
    tickSize: 0.0001,
    minSize: 10,
    maxSize: 100000,
    cryptoBase: "BTC",
    cryptoQuote: "USD"
  }],
  loading: false,
  error: null
};

export default function instrumentsReducer(state = initialState, action) {
  switch (action.type) {
    case FETCH_INSTRUMENTS_BEGIN:
      return {
        ...state,
        loading: true,
        error: null
      };

    case FETCH_INSTRUMENTS_SUCCESS:
      return {
        ...state,
        loading: false,
        instruments: action.payload.instruments
      };

    case FETCH_INSTRUMENTS_FAILURE:
      return {
        ...state,
        loading: false,
        error: action.payload.error,
        instruments: []
      };
    default:
      return state;
  }
}
