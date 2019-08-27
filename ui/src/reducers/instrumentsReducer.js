import {
  FETCH_INSTRUMENTS_BEGIN,
  FETCH_INSTRUMENTS_SUCCESS,
  FETCH_INSTRUMENTS_FAILURE,
  GENERATE_PRICE_CACHE
} from '../actions/instrumentActions';

const initialState = {
  instruments: [],
  prices: [],
  tableMap: {},
  loading: false,
  error: null,
  cached: false
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
        instruments: action.payload.instruments,
        cached: action.payload.cached
      };

    case FETCH_INSTRUMENTS_FAILURE:
      return {
        ...state,
        loading: false,
        error: action.payload.error,
        cached: false,
        instruments: []
      };
    case GENERATE_PRICE_CACHE:
      return {
        ...state,
        prices: action.payload.prices,
        tableMap: action.payload.tableMap
      };
    default:
      return state;
  }
}
