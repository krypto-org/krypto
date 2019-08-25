import React from "react"

import { Layout } from 'antd'
import Sidebar from "./Sidebar"
import Content from "./Content"

class ApplicationLayout extends React.Component {

  render() {
    return (
      <Layout style={{ minHeight: '100vh' }}>
        <Sidebar />
        <Content />
      </Layout>
    )
  }
}

export default ApplicationLayout
