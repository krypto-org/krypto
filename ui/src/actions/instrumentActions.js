import { createInstrumentRequest, requesterSocket, parseInstruments } from "../krypto"

export const FETCH_INSTRUMENTS_BEGIN = "FETCH_INSTRUMENTS_BEGIN";
export const FETCH_INSTRUMENTS_SUCCESS = "FETCH_INSTRUMENTS_SUCCESS";
export const FETCH_INSTRUMENTS_FAILURE = "FETCH_INSTRUMENTS_FAILURE";
export const GENERATE_PRICE_CACHE = "GENERATE_PRICE_CACHE";

export const generatePriceCache = (prices, tableMap) => ({
  type: GENERATE_PRICE_CACHE,
  payload: { prices, tableMap }
});

export const fetchInstrumentsBegin = () => ({
  type: FETCH_INSTRUMENTS_BEGIN
});

export const fetchInstrumentsSuccess = (instruments, cached) => ({
  type: FETCH_INSTRUMENTS_SUCCESS,
  payload: { instruments, cached }
});

export const fetchInstrumentsFailure = error => ({
  type: FETCH_INSTRUMENTS_FAILURE,
  payload: { error }
});

export const fetchInstruments = (forced = false) => {
  return (dispatch, getState) => {
    const cached = getState().instruments.cached;
    if (cached && !forced) {
      dispatch(fetchInstrumentsSuccess(getState().instruments.instruments, cached))
      return
    }
    dispatch(fetchInstrumentsBegin());
    const request = createInstrumentRequest()
    const socket = requesterSocket();
    socket.on("message", (service, payload) => {
      console.log('got reply from ', service.toString(), ":", payload);
      parseInstruments(payload).then(
        instruments => {
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
          Promise.all([dispatch(fetchInstrumentsSuccess(instruments, true)), dispatch(generatePriceCache(prices, tableMap))])
        })
        .catch(error => dispatch(fetchInstrumentsFailure(error)))
    })
    socket.send(["instruments", request.toString()])
  }
}
