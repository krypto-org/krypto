import { createInstrumentRequest, parseInstruments } from "../krypto"
import { requesterSocket } from "../utils/zmq"

const socket = requesterSocket();

export const FETCH_INSTRUMENTS_BEGIN = "FETCH_INSTRUMENTS_BEGIN";
export const FETCH_INSTRUMENTS_SUCCESS = "FETCH_INSTRUMENTS_SUCCESS";
export const FETCH_INSTRUMENTS_FAILURE = "FETCH_INSTRUMENTS_FAILURE";

export const fetchInstrumentsBegin = () => ({ type: FETCH_INSTRUMENTS_BEGIN });

export const fetchInstrumentsSuccess = (instruments, cached) => ({
  type: FETCH_INSTRUMENTS_SUCCESS,
  payload: {
    instruments,
    cached
  }
});

export const fetchInstrumentsFailure = error => ({
  type: FETCH_INSTRUMENTS_FAILURE, payload: {
    error
  }
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
    socket.on("message", (service, payload) => {
      if (service == "instruments") {
        parseInstruments(payload).then(instruments => {
          dispatch(fetchInstrumentsSuccess(instruments, true))
        }).catch(error => dispatch(fetchInstrumentsFailure(error)))
      }
    })
    socket.send([
      "instruments", request.toString()
    ])
  }
}
