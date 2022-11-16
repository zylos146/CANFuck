import { configureStore, combineReducers,  } from '@reduxjs/toolkit';
import createSagaMiddleware from 'redux-saga'
import remoteReducer from './remote_connect/store';
import rootSaga from './saga'

const sagaMiddleware = createSagaMiddleware()

export const store = configureStore({
  reducer: combineReducers({
    remote: remoteReducer
  }),
  middleware: [sagaMiddleware],
  devTools: true
});

sagaMiddleware.run(rootSaga)