
import { createInstrumentsRequest, requesterSocket, parseInstruments } from "../krypto"

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
  console.log("fetchInstruments")
  return dispatch => {
    dispatch(fetchInstrumentsBegin());
    const request = createInstrumentsRequest();
    const socket = requesterSocket();
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
