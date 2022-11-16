import React from 'react'
import PropTypes from 'prop-types'
import { Terminal } from 'xterm'
import { FitAddon } from 'xterm-addon-fit'
import ResizeObserver  from 'react-resize-observer'
import 'xterm/css/xterm.css'

// Based on https://github.com/robert-harbison/xterm-for-react
export default class XTerm extends React.Component {
  constructor(props) {
    super(props)

		this.terminalRef = React.createRef()
		this.xterm = new Terminal(this.props.options)
    this.fitAddon = new FitAddon();
    this.xterm.loadAddon(this.fitAddon);
    this.xterm.write('123456789123456789123456789123456789123456789123456789123456789123456789123456789')
  }

  render() {
    // 
		return <>
      <ResizeObserver onResize={() => {this.fitAddon && this.fitAddon.fit()}} />
      <div className={this.props.className} ref={this.terminalRef}/>
    </>
	}

	componentDidMount() {
		if (this.terminalRef.current) {
			this.xterm.open(this.terminalRef.current)
      this.fitAddon.fit();
		}
	}
  
  componentDidUpdate(prevProps, prevState) {
    if (prevProps.task.logs !== this.state.logs) {
      if (this.state.logs.length === 0) {
        this.xterm.clear();
      }
      for (const log of this.state.logs) {
        /*
        We need to track what we have added to xterm - feels hacky but it's working.
        `this.xterm.clear()` and re-render everything caused screen flicker that's why I decided to not use it.
        Todo: Check if there is a react-xterm wrapper that is not using xterm.clear or 
              create a wrapper component that can render the logs array (with-out flicker).
        */
        if (!this.renderedLogs[log.id]) {
          this.writeln(log.text);
          this.xterm.scrollToBottom();
          this.renderedLogs[log.id] = true;
        }
      }
    }
  }

	componentWillUnmount() {
		this.xterm.dispose()
	}
};

XTerm.propTypes = {
  className: PropTypes.string,
  options: PropTypes.object,
  addons: PropTypes.array,
  onBinary: PropTypes.func,
  onCursorMove: PropTypes.func,
  onData: PropTypes.func,
  onKey: PropTypes.func,
  onLineFeed: PropTypes.func,
  onScroll: PropTypes.func,
  onSelectionChange: PropTypes.func,
  onRender: PropTypes.func,
  onResize: PropTypes.func,
  onTitleChange: PropTypes.func,
  customKeyEventHandler: PropTypes.func,
}