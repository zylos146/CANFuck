import { connect } from 'react-redux'
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import { faCircleCheck } from '@fortawesome/free-solid-svg-icons'
import { setRemoteIp, startConnection } from './actions'

export function RemoteConnect(props) {
  return <span>
    <FontAwesomeIcon icon={faCircleCheck} style={{color: "green"}}/>
    Remote: <input type='text' placeholder='Remote IP' onChange={e => props.setRemoteIp(e.target.value)} value={props.ip}/>
    <button onClick={props.startConnection}>Connect</button>
  </span>
}

const mapState = (state) => {
  return {
    ip: state.remote.ip
  }
}

const mapDispatch = (dispatch) => {
  return {
    startConnection: () => dispatch(startConnection()),
    setRemoteIp: value => dispatch(setRemoteIp(value))
  }
}

export default connect(
  mapState,
  mapDispatch
)(RemoteConnect)