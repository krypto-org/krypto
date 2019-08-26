import { combineReducers } from 'redux';
import globalReducer from "./globalReducer"
import instrumentsReducer from "./instrumentsReducer";

export default combineReducers({
  global: globalReducer,
  instruments: instrumentsReducer
});
