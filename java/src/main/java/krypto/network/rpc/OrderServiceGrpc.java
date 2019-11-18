package krypto.network.rpc;

import static io.grpc.stub.ClientCalls.asyncUnaryCall;
import static io.grpc.stub.ClientCalls.asyncServerStreamingCall;
import static io.grpc.stub.ClientCalls.asyncClientStreamingCall;
import static io.grpc.stub.ClientCalls.asyncBidiStreamingCall;
import static io.grpc.stub.ClientCalls.blockingUnaryCall;
import static io.grpc.stub.ClientCalls.blockingServerStreamingCall;
import static io.grpc.stub.ClientCalls.futureUnaryCall;
import static io.grpc.MethodDescriptor.generateFullMethodName;
import static io.grpc.stub.ServerCalls.asyncUnaryCall;
import static io.grpc.stub.ServerCalls.asyncServerStreamingCall;
import static io.grpc.stub.ServerCalls.asyncClientStreamingCall;
import static io.grpc.stub.ServerCalls.asyncBidiStreamingCall;
import static io.grpc.stub.ServerCalls.asyncUnimplementedUnaryCall;
import static io.grpc.stub.ServerCalls.asyncUnimplementedStreamingCall;

/**
 */
@javax.annotation.Generated(
    value = "by gRPC proto compiler (version 1.0.0-pre2)",
    comments = "Source: krypto.proto")
public class OrderServiceGrpc {

  private OrderServiceGrpc() {}

  public static final String SERVICE_NAME = "krypto.network.rpc.OrderService";

  // Static method descriptors that strictly reflect the proto.
  @io.grpc.ExperimentalApi("https://github.com/grpc/grpc-java/issues/1901")
  public static final io.grpc.MethodDescriptor<krypto.network.rpc.Krypto.OrderRequest,
      krypto.network.rpc.Krypto.OrderUpdate> METHOD_NEW_ORDER =
      io.grpc.MethodDescriptor.create(
          io.grpc.MethodDescriptor.MethodType.UNARY,
          generateFullMethodName(
              "krypto.network.rpc.OrderService", "NewOrder"),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.OrderRequest.getDefaultInstance()),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.OrderUpdate.getDefaultInstance()));
  @io.grpc.ExperimentalApi("https://github.com/grpc/grpc-java/issues/1901")
  public static final io.grpc.MethodDescriptor<krypto.network.rpc.Krypto.OrderCancelRequest,
      krypto.network.rpc.Krypto.OrderUpdate> METHOD_CANCEL_ORDER =
      io.grpc.MethodDescriptor.create(
          io.grpc.MethodDescriptor.MethodType.UNARY,
          generateFullMethodName(
              "krypto.network.rpc.OrderService", "CancelOrder"),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.OrderCancelRequest.getDefaultInstance()),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.OrderUpdate.getDefaultInstance()));
  @io.grpc.ExperimentalApi("https://github.com/grpc/grpc-java/issues/1901")
  public static final io.grpc.MethodDescriptor<krypto.network.rpc.Krypto.OrderReplaceRequest,
      krypto.network.rpc.Krypto.OrderUpdate> METHOD_REPLACE_ORDER =
      io.grpc.MethodDescriptor.create(
          io.grpc.MethodDescriptor.MethodType.UNARY,
          generateFullMethodName(
              "krypto.network.rpc.OrderService", "ReplaceOrder"),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.OrderReplaceRequest.getDefaultInstance()),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.OrderUpdate.getDefaultInstance()));
  @io.grpc.ExperimentalApi("https://github.com/grpc/grpc-java/issues/1901")
  public static final io.grpc.MethodDescriptor<krypto.network.rpc.Krypto.Request,
      krypto.network.rpc.Krypto.Order> METHOD_GET_ORDERS =
      io.grpc.MethodDescriptor.create(
          io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING,
          generateFullMethodName(
              "krypto.network.rpc.OrderService", "GetOrders"),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.Request.getDefaultInstance()),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.Order.getDefaultInstance()));

  /**
   * Creates a new async stub that supports all call types for the service
   */
  public static OrderServiceStub newStub(io.grpc.Channel channel) {
    return new OrderServiceStub(channel);
  }

  /**
   * Creates a new blocking-style stub that supports unary and streaming output calls on the service
   */
  public static OrderServiceBlockingStub newBlockingStub(
      io.grpc.Channel channel) {
    return new OrderServiceBlockingStub(channel);
  }

  /**
   * Creates a new ListenableFuture-style stub that supports unary and streaming output calls on the service
   */
  public static OrderServiceFutureStub newFutureStub(
      io.grpc.Channel channel) {
    return new OrderServiceFutureStub(channel);
  }

  /**
   */
  public static abstract class OrderServiceImplBase implements io.grpc.BindableService {

    /**
     */
    public void newOrder(krypto.network.rpc.Krypto.OrderRequest request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.OrderUpdate> responseObserver) {
      asyncUnimplementedUnaryCall(METHOD_NEW_ORDER, responseObserver);
    }

    /**
     */
    public void cancelOrder(krypto.network.rpc.Krypto.OrderCancelRequest request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.OrderUpdate> responseObserver) {
      asyncUnimplementedUnaryCall(METHOD_CANCEL_ORDER, responseObserver);
    }

    /**
     */
    public void replaceOrder(krypto.network.rpc.Krypto.OrderReplaceRequest request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.OrderUpdate> responseObserver) {
      asyncUnimplementedUnaryCall(METHOD_REPLACE_ORDER, responseObserver);
    }

    /**
     */
    public void getOrders(krypto.network.rpc.Krypto.Request request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.Order> responseObserver) {
      asyncUnimplementedUnaryCall(METHOD_GET_ORDERS, responseObserver);
    }

    @java.lang.Override public io.grpc.ServerServiceDefinition bindService() {
      return io.grpc.ServerServiceDefinition.builder(getServiceDescriptor())
          .addMethod(
            METHOD_NEW_ORDER,
            asyncUnaryCall(
              new MethodHandlers<
                krypto.network.rpc.Krypto.OrderRequest,
                krypto.network.rpc.Krypto.OrderUpdate>(
                  this, METHODID_NEW_ORDER)))
          .addMethod(
            METHOD_CANCEL_ORDER,
            asyncUnaryCall(
              new MethodHandlers<
                krypto.network.rpc.Krypto.OrderCancelRequest,
                krypto.network.rpc.Krypto.OrderUpdate>(
                  this, METHODID_CANCEL_ORDER)))
          .addMethod(
            METHOD_REPLACE_ORDER,
            asyncUnaryCall(
              new MethodHandlers<
                krypto.network.rpc.Krypto.OrderReplaceRequest,
                krypto.network.rpc.Krypto.OrderUpdate>(
                  this, METHODID_REPLACE_ORDER)))
          .addMethod(
            METHOD_GET_ORDERS,
            asyncServerStreamingCall(
              new MethodHandlers<
                krypto.network.rpc.Krypto.Request,
                krypto.network.rpc.Krypto.Order>(
                  this, METHODID_GET_ORDERS)))
          .build();
    }
  }

  /**
   */
  public static final class OrderServiceStub extends io.grpc.stub.AbstractStub<OrderServiceStub> {
    private OrderServiceStub(io.grpc.Channel channel) {
      super(channel);
    }

    private OrderServiceStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected OrderServiceStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new OrderServiceStub(channel, callOptions);
    }

    /**
     */
    public void newOrder(krypto.network.rpc.Krypto.OrderRequest request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.OrderUpdate> responseObserver) {
      asyncUnaryCall(
          getChannel().newCall(METHOD_NEW_ORDER, getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void cancelOrder(krypto.network.rpc.Krypto.OrderCancelRequest request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.OrderUpdate> responseObserver) {
      asyncUnaryCall(
          getChannel().newCall(METHOD_CANCEL_ORDER, getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void replaceOrder(krypto.network.rpc.Krypto.OrderReplaceRequest request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.OrderUpdate> responseObserver) {
      asyncUnaryCall(
          getChannel().newCall(METHOD_REPLACE_ORDER, getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void getOrders(krypto.network.rpc.Krypto.Request request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.Order> responseObserver) {
      asyncServerStreamingCall(
          getChannel().newCall(METHOD_GET_ORDERS, getCallOptions()), request, responseObserver);
    }
  }

  /**
   */
  public static final class OrderServiceBlockingStub extends io.grpc.stub.AbstractStub<OrderServiceBlockingStub> {
    private OrderServiceBlockingStub(io.grpc.Channel channel) {
      super(channel);
    }

    private OrderServiceBlockingStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected OrderServiceBlockingStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new OrderServiceBlockingStub(channel, callOptions);
    }

    /**
     */
    public krypto.network.rpc.Krypto.OrderUpdate newOrder(krypto.network.rpc.Krypto.OrderRequest request) {
      return blockingUnaryCall(
          getChannel(), METHOD_NEW_ORDER, getCallOptions(), request);
    }

    /**
     */
    public krypto.network.rpc.Krypto.OrderUpdate cancelOrder(krypto.network.rpc.Krypto.OrderCancelRequest request) {
      return blockingUnaryCall(
          getChannel(), METHOD_CANCEL_ORDER, getCallOptions(), request);
    }

    /**
     */
    public krypto.network.rpc.Krypto.OrderUpdate replaceOrder(krypto.network.rpc.Krypto.OrderReplaceRequest request) {
      return blockingUnaryCall(
          getChannel(), METHOD_REPLACE_ORDER, getCallOptions(), request);
    }

    /**
     */
    public java.util.Iterator<krypto.network.rpc.Krypto.Order> getOrders(
        krypto.network.rpc.Krypto.Request request) {
      return blockingServerStreamingCall(
          getChannel(), METHOD_GET_ORDERS, getCallOptions(), request);
    }
  }

  /**
   */
  public static final class OrderServiceFutureStub extends io.grpc.stub.AbstractStub<OrderServiceFutureStub> {
    private OrderServiceFutureStub(io.grpc.Channel channel) {
      super(channel);
    }

    private OrderServiceFutureStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected OrderServiceFutureStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new OrderServiceFutureStub(channel, callOptions);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<krypto.network.rpc.Krypto.OrderUpdate> newOrder(
        krypto.network.rpc.Krypto.OrderRequest request) {
      return futureUnaryCall(
          getChannel().newCall(METHOD_NEW_ORDER, getCallOptions()), request);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<krypto.network.rpc.Krypto.OrderUpdate> cancelOrder(
        krypto.network.rpc.Krypto.OrderCancelRequest request) {
      return futureUnaryCall(
          getChannel().newCall(METHOD_CANCEL_ORDER, getCallOptions()), request);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<krypto.network.rpc.Krypto.OrderUpdate> replaceOrder(
        krypto.network.rpc.Krypto.OrderReplaceRequest request) {
      return futureUnaryCall(
          getChannel().newCall(METHOD_REPLACE_ORDER, getCallOptions()), request);
    }
  }

  private static final int METHODID_NEW_ORDER = 0;
  private static final int METHODID_CANCEL_ORDER = 1;
  private static final int METHODID_REPLACE_ORDER = 2;
  private static final int METHODID_GET_ORDERS = 3;

  private static class MethodHandlers<Req, Resp> implements
      io.grpc.stub.ServerCalls.UnaryMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ServerStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ClientStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.BidiStreamingMethod<Req, Resp> {
    private final OrderServiceImplBase serviceImpl;
    private final int methodId;

    public MethodHandlers(OrderServiceImplBase serviceImpl, int methodId) {
      this.serviceImpl = serviceImpl;
      this.methodId = methodId;
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public void invoke(Req request, io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        case METHODID_NEW_ORDER:
          serviceImpl.newOrder((krypto.network.rpc.Krypto.OrderRequest) request,
              (io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.OrderUpdate>) responseObserver);
          break;
        case METHODID_CANCEL_ORDER:
          serviceImpl.cancelOrder((krypto.network.rpc.Krypto.OrderCancelRequest) request,
              (io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.OrderUpdate>) responseObserver);
          break;
        case METHODID_REPLACE_ORDER:
          serviceImpl.replaceOrder((krypto.network.rpc.Krypto.OrderReplaceRequest) request,
              (io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.OrderUpdate>) responseObserver);
          break;
        case METHODID_GET_ORDERS:
          serviceImpl.getOrders((krypto.network.rpc.Krypto.Request) request,
              (io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.Order>) responseObserver);
          break;
        default:
          throw new AssertionError();
      }
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public io.grpc.stub.StreamObserver<Req> invoke(
        io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        default:
          throw new AssertionError();
      }
    }
  }

  public static io.grpc.ServiceDescriptor getServiceDescriptor() {
    return new io.grpc.ServiceDescriptor(SERVICE_NAME,
        METHOD_NEW_ORDER,
        METHOD_CANCEL_ORDER,
        METHOD_REPLACE_ORDER,
        METHOD_GET_ORDERS);
  }

}
