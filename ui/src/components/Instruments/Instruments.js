import React, { Component } from "react";
import styles from "./Instruments.scss"
import { Row, Col, Tag, Alert } from 'antd';
import { connect } from 'react-redux';
import ReactTable from "react-table";
import PropTypes from "prop-types"
import "react-table/react-table.css";

class Instruments extends Component {
  render() {
    return (
      <div className={styles.instruments}>
        <Row>
          <Col span={4}>
            <Tag color={this.props.loaded ? "green" : "red"} style={{ margin: "10px 0" }}>
              {this.props.loaded ? "LOADED" : "ERRORED"}
            </Tag>
          </Col>
          <Col span={20}>
            {this.props.error && <Alert message={this.props.error.message} type="error" />}
          </Col>
        </Row>
        <Row>
          <Col span={24}>
            <ReactTable
              data={this.props.instruments}
              columns={[
                {
                  Header: "Id",
                  accessor: "id"
                },
                {
                  Header: "Type",
                  accessor: "type"
                },
                {
                  Header: "Symbol",
                  accessor: "symbol"
                },
                {
                  Header: "Exchange",
                  accessor: "exchange"
                },
                {
                  Header: "Exchange Symbol",
                  accessor: "exchangeSymbol"
                },
                {
                  Header: "Tick Size",
                  accessor: "tickSize"
                },
                {
                  Header: "Min Size",
                  accessor: "minSize"
                },
                {
                  Header: "Max Size",
                  accessor: "maxSize"
                }
              ]}
              defaultPageSize={25}
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

Instruments.propTypes = {
  instruments: PropTypes.array.isRequired,
  error: PropTypes.object,
  loaded: PropTypes.bool.isRequired
}

const mapStateToProps = (state, ownProps) => {
  return {
    ...ownProps,
    instruments: state.instruments.instruments,
    error: state.instruments.error,
    loaded: (!state.instruments.loading) && (!state.instruments.error)
  }
}

export default connect(mapStateToProps)(Instruments);
