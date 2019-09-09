import React, { Component } from "react";
import { Row, Col, Tag } from 'antd';
import styles from "./Prices.scss"

import PropTypes from "prop-types"
import { connect } from "react-redux"

import { onInstrumentsCached, openWebsocketConnection, closeWebsocketConnection } from "../../actions/mktdataActions"

import ReactTable from "react-table";
import "react-table/react-table.css";

import { wsIpAddress } from "../../krypto/"

const websocketUrl = `ws://${wsIpAddress}:5757`

class Prices extends Component {

  componentDidUpdate() {
    if (!this.props.mdInitialized && this.props.instrumentsConnected) {
      this.props.onInstrumentsCached(this.props.instruments)
    }

    if (this.props.mdInitialized && this.props.instrumentsConnected) {
      this.props.openWebsocketConnection(websocketUrl);
    }
  }

  render() {

    const prices = Object.keys(this.props.prices).map((key) => this.props.prices[key])
    return (
      <div className={styles.prices}>
        <Row>
          <Col span={4}>
            <Tag color={this.props.instrumentsConnected ? "green" : "red"} style={{ margin: "10px 0" }}>
              INSTRUMENTS
            </Tag>
          </Col>
          <Col span={4}>
            <Tag color={this.props.mdFetching ? "green" : this.props.mdInitialized ? "yellow" : "red"} style={{ margin: "10px 0" }}>
              MKTDATA
            </Tag>
          </Col>
          <Col span={20}></Col>
        </Row>
        <Row>
          <Col span={24}>
            <ReactTable
              data={prices}
              columns={[
                {
                  Header: "Symbol",
                  accessor: "symbol"
                },
                {
                  Header: "Bid",
                  accessor: d => isNaN(d.bid) ? "NaN" : d.bid.toFixed(4),
                  id: "bid"
                },
                {
                  Header: "Bid Qty",
                  accessor: d => d.bid_quantity.toFixed(4),
                  id: "bid_quantity"
                },
                {
                  Header: "Ask Qty",
                  accessor: d => d.ask_quantity.toFixed(4),
                  id: "ask_quantity"
                },
                {
                  Header: "Ask",
                  accessor: d => isNaN(d.ask) ? "NaN" : d.ask.toFixed(4),
                  id: "ask"
                },
                {
                  Header: "Last",
                  accessor: d => isNaN(d.last) ? "NaN" : d.last.toFixed(4),
                  id: "last"
                },
                {
                  Header: "Last Qty",
                  accessor: d => d.last_quantity.toFixed(4),
                  id: "last_quantity"
                }
              ]}
              defaultPageSize={100}
              className="-striped -highlight"
              getTdProps={(state, rowInfo, column) => {
                if (column.id != "symbol") {
                  return ({
                    style: {
                      textAlign: 'right'
                    }
                  })
                } else {
                  return {}
                }
              }}
              style={{
                height: "800px",
                width: "100%"
              }}
            />
          </Col>
        </Row>
      </div>);
  }
}

Prices.propTypes = {
  instruments: PropTypes.array.isRequired,
  prices: PropTypes.object.isRequired,
  instrumentsConnected: PropTypes.bool.isRequired,
  mdInitialized: PropTypes.bool.isRequired,
  mdFetching: PropTypes.bool.isRequired,
  onInstrumentsCached: PropTypes.func.isRequired,
  openWebsocketConnection: PropTypes.func.isRequired,
  closeWebsocketConnection: PropTypes.func.isRequired,
}

const mapStateToProps = (state, ownProps) => {
  return {
    ...ownProps,
    instruments: state.instruments.instruments,
    instrumentsConnected: state.instruments.cached,
    prices: state.mktdata.prices,
    mdInitialized: state.mktdata.initialized,
    mdFetching: state.mktdata.connected,
  }
}

const mapDispatchToProps = (dispatch) => {
  return {
    onInstrumentsCached: (instruments) => dispatch(onInstrumentsCached(instruments)),
    openWebsocketConnection: (url) => dispatch(openWebsocketConnection(url)),
    closeWebsocketConnection: (url) => dispatch(closeWebsocketConnection(url)),
  }
}

export default connect(mapStateToProps, mapDispatchToProps)(Prices);
