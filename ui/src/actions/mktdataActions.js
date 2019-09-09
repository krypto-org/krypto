import {
  WEBSOCKET_CLOSED,
  WEBSOCKET_CONNECT,
  WEBSOCKET_DISCONNECT,
  WEBSOCKET_MESSAGE,
  WEBSOCKET_OPEN,
  WEBSOCKET_SEND,
  connect,
  disconnect,
} from '@giantmachines/redux-websocket';

import { WEBSOCKET_PREFIX } from '../constants';

export const GENERATE_PRICE_CACHE = "GENERATE_PRICE_CACHE";

export const REDUX_WEBSOCKET_OPEN = `${WEBSOCKET_PREFIX}::${WEBSOCKET_OPEN}`
export const REDUX_WEBSOCKET_CLOSED = `${WEBSOCKET_PREFIX}::${WEBSOCKET_CLOSED}`
export const REDUX_WEBSOCKET_MESSAGE = `${WEBSOCKET_PREFIX}::${WEBSOCKET_MESSAGE}`
export const REDUX_WEBSOCKET_CONNECT = `${WEBSOCKET_PREFIX}::${WEBSOCKET_CONNECT}`
export const REDUX_WEBSOCKET_DISCONNECT = `${WEBSOCKET_PREFIX}::${WEBSOCKET_DISCONNECT}`
export const REDUX_WEBSOCKET_SEND = `${WEBSOCKET_PREFIX}::${WEBSOCKET_SEND}`

export const generatePriceCache = (prices, tableMap) => ({
  type: GENERATE_PRICE_CACHE,
  payload: {
    prices,
    tableMap
  }
});

export const websocketSend = payload => ({
  type: REDUX_WEBSOCKET_SEND,
  payload,
});


export const onInstrumentsCached = (instruments) => {
  return (dispatch) => {
    const prices = instruments.map(e => ({
      symbol: e.symbol + "@" + e.exchange,
      bid: NaN,
      bid_quantity: 0,
      ask: NaN,
      ask_quantity: 0,
      last: NaN,
      last_quantity: 0
    }))
    const tableMap = instruments.reduce((r, e, i) => {
      r[e.id.toString()] = i
      return r
    }, {})
    console.log(`Dispatched: ${prices.length}`)
    dispatch(generatePriceCache(prices, tableMap))
  }
}

export const openWebsocketConnection = (url) => {
  return (dispatch, getState) => {
    if (!getState().mktdata.connected) {
      dispatch(connect(url, WEBSOCKET_PREFIX))
    }
  }
}

export const closeWebsocketConnection = (url) => {
  return (dispatch) => {
    dispatch(disconnect(url, WEBSOCKET_PREFIX))
  }
}
