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
public class PriceCacheServiceGrpc {

  private PriceCacheServiceGrpc() {}

  public static final String SERVICE_NAME = "krypto.network.rpc.PriceCacheService";

  // Static method descriptors that strictly reflect the proto.
  @io.grpc.ExperimentalApi("https://github.com/grpc/grpc-java/issues/1901")
  public static final io.grpc.MethodDescriptor<krypto.network.rpc.Krypto.Request,
      krypto.network.rpc.Krypto.Quote> METHOD_GET_QUOTES =
      io.grpc.MethodDescriptor.create(
          io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING,
          generateFullMethodName(
              "krypto.network.rpc.PriceCacheService", "GetQuotes"),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.Request.getDefaultInstance()),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.Quote.getDefaultInstance()));

  /**
   * Creates a new async stub that supports all call types for the service
   */
  public static PriceCacheServiceStub newStub(io.grpc.Channel channel) {
    return new PriceCacheServiceStub(channel);
  }

  /**
   * Creates a new blocking-style stub that supports unary and streaming output calls on the service
   */
  public static PriceCacheServiceBlockingStub newBlockingStub(
      io.grpc.Channel channel) {
    return new PriceCacheServiceBlockingStub(channel);
  }

  /**
   * Creates a new ListenableFuture-style stub that supports unary and streaming output calls on the service
   */
  public static PriceCacheServiceFutureStub newFutureStub(
      io.grpc.Channel channel) {
    return new PriceCacheServiceFutureStub(channel);
  }

  /**
   */
  public static abstract class PriceCacheServiceImplBase implements io.grpc.BindableService {

    /**
     */
    public void getQuotes(krypto.network.rpc.Krypto.Request request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.Quote> responseObserver) {
      asyncUnimplementedUnaryCall(METHOD_GET_QUOTES, responseObserver);
    }

    @java.lang.Override public io.grpc.ServerServiceDefinition bindService() {
      return io.grpc.ServerServiceDefinition.builder(getServiceDescriptor())
          .addMethod(
            METHOD_GET_QUOTES,
            asyncServerStreamingCall(
              new MethodHandlers<
                krypto.network.rpc.Krypto.Request,
                krypto.network.rpc.Krypto.Quote>(
                  this, METHODID_GET_QUOTES)))
          .build();
    }
  }

  /**
   */
  public static final class PriceCacheServiceStub extends io.grpc.stub.AbstractStub<PriceCacheServiceStub> {
    private PriceCacheServiceStub(io.grpc.Channel channel) {
      super(channel);
    }

    private PriceCacheServiceStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected PriceCacheServiceStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new PriceCacheServiceStub(channel, callOptions);
    }

    /**
     */
    public void getQuotes(krypto.network.rpc.Krypto.Request request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.Quote> responseObserver) {
      asyncServerStreamingCall(
          getChannel().newCall(METHOD_GET_QUOTES, getCallOptions()), request, responseObserver);
    }
  }

  /**
   */
  public static final class PriceCacheServiceBlockingStub extends io.grpc.stub.AbstractStub<PriceCacheServiceBlockingStub> {
    private PriceCacheServiceBlockingStub(io.grpc.Channel channel) {
      super(channel);
    }

    private PriceCacheServiceBlockingStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected PriceCacheServiceBlockingStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new PriceCacheServiceBlockingStub(channel, callOptions);
    }

    /**
     */
    public java.util.Iterator<krypto.network.rpc.Krypto.Quote> getQuotes(
        krypto.network.rpc.Krypto.Request request) {
      return blockingServerStreamingCall(
          getChannel(), METHOD_GET_QUOTES, getCallOptions(), request);
    }
  }

  /**
   */
  public static final class PriceCacheServiceFutureStub extends io.grpc.stub.AbstractStub<PriceCacheServiceFutureStub> {
    private PriceCacheServiceFutureStub(io.grpc.Channel channel) {
      super(channel);
    }

    private PriceCacheServiceFutureStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected PriceCacheServiceFutureStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new PriceCacheServiceFutureStub(channel, callOptions);
    }
  }

  private static final int METHODID_GET_QUOTES = 0;

  private static class MethodHandlers<Req, Resp> implements
      io.grpc.stub.ServerCalls.UnaryMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ServerStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ClientStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.BidiStreamingMethod<Req, Resp> {
    private final PriceCacheServiceImplBase serviceImpl;
    private final int methodId;

    public MethodHandlers(PriceCacheServiceImplBase serviceImpl, int methodId) {
      this.serviceImpl = serviceImpl;
      this.methodId = methodId;
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public void invoke(Req request, io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        case METHODID_GET_QUOTES:
          serviceImpl.getQuotes((krypto.network.rpc.Krypto.Request) request,
              (io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.Quote>) responseObserver);
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
        METHOD_GET_QUOTES);
  }

}
