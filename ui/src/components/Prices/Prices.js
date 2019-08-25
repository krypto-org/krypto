import React, { Component } from "react";
import { Row, Col, Tag } from 'antd';
import styles from "./Prices.scss"

import ReactTable from "react-table";
import "react-table/react-table.css";

class Prices extends Component {
  constructor(props) {
    super(props);
    this.state = {
      prices: [],
      connected: false
    };
  }

  render() {
    return (
      <div className={styles.prices}>
        <Row>
          <Col span={4}>
            <Tag color={this.state.connected ? "green" : "red"} style={{ margin: "10px 0" }}>
              {this.state.connected ? "CONNECTED" : "DISCONNECTED"}
            </Tag>
          </Col>
          <Col span={20}></Col>
        </Row>
        <Row>
          <Col span={24}>
            <ReactTable
              data={this.state.prices}
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

export default Prices;
