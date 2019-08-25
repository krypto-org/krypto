import { combineReducers } from 'redux';
import instruments from "./instrumentsReducer";

export default combineReducers({
  instruments: instruments
});
