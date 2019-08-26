import { createInstrumentRequest, requesterSocket, parseInstruments } from "../krypto"

export const FETCH_INSTRUMENTS_BEGIN = "FETCH_INSTRUMENTS_BEGIN";
export const FETCH_INSTRUMENTS_SUCCESS = "FETCH_INSTRUMENTS_SUCCESS";
export const FETCH_INSTRUMENTS_FAILURE = "FETCH_INSTRUMENTS_FAILURE";

export const fetchInstrumentsBegin = () => ({
  type: FETCH_INSTRUMENTS_BEGIN
});

export const fetchInstrumentsSuccess = instruments => ({
  type: FETCH_INSTRUMENTS_SUCCESS,
  payload: { instruments }
});

export const fetchInstrumentsFailure = error => ({
  type: FETCH_INSTRUMENTS_FAILURE,
  payload: { error }
});

export const fetchInstruments = () => {
  return (dispatch) => {
    dispatch(fetchInstrumentsBegin());
    const request = createInstrumentRequest()
    console.log("created request")
    const socket = requesterSocket();
    console.log("created socket")
    socket.on("message", (service, payload) => {
      console.log('got reply from ', service.toString(), ":", payload);
      parseInstruments(payload).then(
        instruments =>
          dispatch(fetchInstrumentsSuccess(instruments)))
        .catch(error => dispatch(fetchInstrumentsFailure(error)))
    })
    socket.send(["instruments", request.toString()])
  }
}
