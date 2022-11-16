import { all } from 'redux-saga/effects'
import remoteSagas from './remote_connect/saga'

function* rootSaga() {
  yield all([
    ...remoteSagas
  ]);
}

export default rootSaga;