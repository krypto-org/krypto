import { combineReducers } from 'redux';
import globalReducer from "./globalReducer"
import instrumentsReducer from "./instrumentsReducer";
import mktdataReducer from "./mktdataReducer";

export default combineReducers({
  global: globalReducer,
  instruments: instrumentsReducer,
  mktdata: mktdataReducer
});
