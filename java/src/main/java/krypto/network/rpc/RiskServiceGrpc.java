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
public class RiskServiceGrpc {

  private RiskServiceGrpc() {}

  public static final String SERVICE_NAME = "krypto.network.rpc.RiskService";

  // Static method descriptors that strictly reflect the proto.
  @io.grpc.ExperimentalApi("https://github.com/grpc/grpc-java/issues/1901")
  public static final io.grpc.MethodDescriptor<krypto.network.rpc.Krypto.Request,
      krypto.network.rpc.Krypto.Position> METHOD_GET_POSITIONS =
      io.grpc.MethodDescriptor.create(
          io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING,
          generateFullMethodName(
              "krypto.network.rpc.RiskService", "GetPositions"),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.Request.getDefaultInstance()),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.Position.getDefaultInstance()));
  @io.grpc.ExperimentalApi("https://github.com/grpc/grpc-java/issues/1901")
  public static final io.grpc.MethodDescriptor<krypto.network.rpc.Krypto.Request,
      krypto.network.rpc.Krypto.RiskSummary> METHOD_GET_RISK_SUMMARY =
      io.grpc.MethodDescriptor.create(
          io.grpc.MethodDescriptor.MethodType.UNARY,
          generateFullMethodName(
              "krypto.network.rpc.RiskService", "GetRiskSummary"),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.Request.getDefaultInstance()),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.RiskSummary.getDefaultInstance()));
  @io.grpc.ExperimentalApi("https://github.com/grpc/grpc-java/issues/1901")
  public static final io.grpc.MethodDescriptor<krypto.network.rpc.Krypto.Request,
      krypto.network.rpc.Krypto.RiskSummary> METHOD_GET_RISK_SUMMARIES =
      io.grpc.MethodDescriptor.create(
          io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING,
          generateFullMethodName(
              "krypto.network.rpc.RiskService", "GetRiskSummaries"),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.Request.getDefaultInstance()),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.RiskSummary.getDefaultInstance()));

  /**
   * Creates a new async stub that supports all call types for the service
   */
  public static RiskServiceStub newStub(io.grpc.Channel channel) {
    return new RiskServiceStub(channel);
  }

  /**
   * Creates a new blocking-style stub that supports unary and streaming output calls on the service
   */
  public static RiskServiceBlockingStub newBlockingStub(
      io.grpc.Channel channel) {
    return new RiskServiceBlockingStub(channel);
  }

  /**
   * Creates a new ListenableFuture-style stub that supports unary and streaming output calls on the service
   */
  public static RiskServiceFutureStub newFutureStub(
      io.grpc.Channel channel) {
    return new RiskServiceFutureStub(channel);
  }

  /**
   */
  public static abstract class RiskServiceImplBase implements io.grpc.BindableService {

    /**
     */
    public void getPositions(krypto.network.rpc.Krypto.Request request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.Position> responseObserver) {
      asyncUnimplementedUnaryCall(METHOD_GET_POSITIONS, responseObserver);
    }

    /**
     */
    public void getRiskSummary(krypto.network.rpc.Krypto.Request request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.RiskSummary> responseObserver) {
      asyncUnimplementedUnaryCall(METHOD_GET_RISK_SUMMARY, responseObserver);
    }

    /**
     */
    public void getRiskSummaries(krypto.network.rpc.Krypto.Request request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.RiskSummary> responseObserver) {
      asyncUnimplementedUnaryCall(METHOD_GET_RISK_SUMMARIES, responseObserver);
    }

    @java.lang.Override public io.grpc.ServerServiceDefinition bindService() {
      return io.grpc.ServerServiceDefinition.builder(getServiceDescriptor())
          .addMethod(
            METHOD_GET_POSITIONS,
            asyncServerStreamingCall(
              new MethodHandlers<
                krypto.network.rpc.Krypto.Request,
                krypto.network.rpc.Krypto.Position>(
                  this, METHODID_GET_POSITIONS)))
          .addMethod(
            METHOD_GET_RISK_SUMMARY,
            asyncUnaryCall(
              new MethodHandlers<
                krypto.network.rpc.Krypto.Request,
                krypto.network.rpc.Krypto.RiskSummary>(
                  this, METHODID_GET_RISK_SUMMARY)))
          .addMethod(
            METHOD_GET_RISK_SUMMARIES,
            asyncServerStreamingCall(
              new MethodHandlers<
                krypto.network.rpc.Krypto.Request,
                krypto.network.rpc.Krypto.RiskSummary>(
                  this, METHODID_GET_RISK_SUMMARIES)))
          .build();
    }
  }

  /**
   */
  public static final class RiskServiceStub extends io.grpc.stub.AbstractStub<RiskServiceStub> {
    private RiskServiceStub(io.grpc.Channel channel) {
      super(channel);
    }

    private RiskServiceStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected RiskServiceStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new RiskServiceStub(channel, callOptions);
    }

    /**
     */
    public void getPositions(krypto.network.rpc.Krypto.Request request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.Position> responseObserver) {
      asyncServerStreamingCall(
          getChannel().newCall(METHOD_GET_POSITIONS, getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void getRiskSummary(krypto.network.rpc.Krypto.Request request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.RiskSummary> responseObserver) {
      asyncUnaryCall(
          getChannel().newCall(METHOD_GET_RISK_SUMMARY, getCallOptions()), request, responseObserver);
    }

    /**
     */
    public void getRiskSummaries(krypto.network.rpc.Krypto.Request request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.RiskSummary> responseObserver) {
      asyncServerStreamingCall(
          getChannel().newCall(METHOD_GET_RISK_SUMMARIES, getCallOptions()), request, responseObserver);
    }
  }

  /**
   */
  public static final class RiskServiceBlockingStub extends io.grpc.stub.AbstractStub<RiskServiceBlockingStub> {
    private RiskServiceBlockingStub(io.grpc.Channel channel) {
      super(channel);
    }

    private RiskServiceBlockingStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected RiskServiceBlockingStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new RiskServiceBlockingStub(channel, callOptions);
    }

    /**
     */
    public java.util.Iterator<krypto.network.rpc.Krypto.Position> getPositions(
        krypto.network.rpc.Krypto.Request request) {
      return blockingServerStreamingCall(
          getChannel(), METHOD_GET_POSITIONS, getCallOptions(), request);
    }

    /**
     */
    public krypto.network.rpc.Krypto.RiskSummary getRiskSummary(krypto.network.rpc.Krypto.Request request) {
      return blockingUnaryCall(
          getChannel(), METHOD_GET_RISK_SUMMARY, getCallOptions(), request);
    }

    /**
     */
    public java.util.Iterator<krypto.network.rpc.Krypto.RiskSummary> getRiskSummaries(
        krypto.network.rpc.Krypto.Request request) {
      return blockingServerStreamingCall(
          getChannel(), METHOD_GET_RISK_SUMMARIES, getCallOptions(), request);
    }
  }

  /**
   */
  public static final class RiskServiceFutureStub extends io.grpc.stub.AbstractStub<RiskServiceFutureStub> {
    private RiskServiceFutureStub(io.grpc.Channel channel) {
      super(channel);
    }

    private RiskServiceFutureStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected RiskServiceFutureStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new RiskServiceFutureStub(channel, callOptions);
    }

    /**
     */
    public com.google.common.util.concurrent.ListenableFuture<krypto.network.rpc.Krypto.RiskSummary> getRiskSummary(
        krypto.network.rpc.Krypto.Request request) {
      return futureUnaryCall(
          getChannel().newCall(METHOD_GET_RISK_SUMMARY, getCallOptions()), request);
    }
  }

  private static final int METHODID_GET_POSITIONS = 0;
  private static final int METHODID_GET_RISK_SUMMARY = 1;
  private static final int METHODID_GET_RISK_SUMMARIES = 2;

  private static class MethodHandlers<Req, Resp> implements
      io.grpc.stub.ServerCalls.UnaryMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ServerStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ClientStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.BidiStreamingMethod<Req, Resp> {
    private final RiskServiceImplBase serviceImpl;
    private final int methodId;

    public MethodHandlers(RiskServiceImplBase serviceImpl, int methodId) {
      this.serviceImpl = serviceImpl;
      this.methodId = methodId;
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public void invoke(Req request, io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        case METHODID_GET_POSITIONS:
          serviceImpl.getPositions((krypto.network.rpc.Krypto.Request) request,
              (io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.Position>) responseObserver);
          break;
        case METHODID_GET_RISK_SUMMARY:
          serviceImpl.getRiskSummary((krypto.network.rpc.Krypto.Request) request,
              (io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.RiskSummary>) responseObserver);
          break;
        case METHODID_GET_RISK_SUMMARIES:
          serviceImpl.getRiskSummaries((krypto.network.rpc.Krypto.Request) request,
              (io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.RiskSummary>) responseObserver);
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
        METHOD_GET_POSITIONS,
        METHOD_GET_RISK_SUMMARY,
        METHOD_GET_RISK_SUMMARIES);
  }

}
