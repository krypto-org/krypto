import React, { Component } from "react";
import { Row, Col, Tag } from 'antd';
import styles from "./Prices.scss"

import PropTypes from "prop-types"
import { connect } from "react-redux"

import ReactTable from "react-table";
import "react-table/react-table.css";

class Prices extends Component {
  componentDidMount() {
    console.log(this.props)
  }

  render() {
    return (
      <div className={styles.prices}>
        <Row>
          <Col span={4}>
            <Tag color={this.props.connected ? "green" : "red"} style={{ margin: "10px 0" }}>
              {this.props.connected ? "CONNECTED" : "DISCONNECTED"}
            </Tag>
          </Col>
          <Col span={20}></Col>
        </Row>
        <Row>
          <Col span={24}>
            <ReactTable
              data={this.props.prices}
              columns={[
                {
                  Header: "Symbol",
                  accessor: "symbol"
                },
                {
                  Header: "Bid",
                  accessor: "bid"
                },
                {
                  Header: "Bid Qty",
                  accessor: "bid_quantity"
                },
                {
                  Header: "Ask Qty",
                  accessor: "ask_quantity"
                },
                {
                  Header: "Ask",
                  accessor: "ask"
                },
                {
                  Header: "Last",
                  accessor: "last"
                },
                {
                  Header: "Last Qty",
                  accessor: "last_quantity"
                }
              ]}
              defaultPageSize={100}
              className="-striped -highlight"
              getTdProps={() => ({
                style: {
                  textAlign: 'center'
                }
              })}
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
  prices: PropTypes.array.isRequired,
  tableMap: PropTypes.object.isRequired,
  connected: PropTypes.bool.isRequired
}

const mapStateToProps = (state, ownProps) => {
  return {
    ...ownProps,
    instruments: state.instruments.instruments,
    prices: state.instruments.prices,
    tableMap: state.instruments.tableMap,
    connected: false
  }
}

export default connect(mapStateToProps)(Prices);
