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
public class InstrumentServiceGrpc {

  private InstrumentServiceGrpc() {}

  public static final String SERVICE_NAME = "krypto.network.rpc.InstrumentService";

  // Static method descriptors that strictly reflect the proto.
  @io.grpc.ExperimentalApi("https://github.com/grpc/grpc-java/issues/1901")
  public static final io.grpc.MethodDescriptor<krypto.network.rpc.Krypto.Request,
      krypto.network.rpc.Krypto.Instrument> METHOD_GET_INSTRUMENTS =
      io.grpc.MethodDescriptor.create(
          io.grpc.MethodDescriptor.MethodType.SERVER_STREAMING,
          generateFullMethodName(
              "krypto.network.rpc.InstrumentService", "GetInstruments"),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.Request.getDefaultInstance()),
          io.grpc.protobuf.ProtoUtils.marshaller(krypto.network.rpc.Krypto.Instrument.getDefaultInstance()));

  /**
   * Creates a new async stub that supports all call types for the service
   */
  public static InstrumentServiceStub newStub(io.grpc.Channel channel) {
    return new InstrumentServiceStub(channel);
  }

  /**
   * Creates a new blocking-style stub that supports unary and streaming output calls on the service
   */
  public static InstrumentServiceBlockingStub newBlockingStub(
      io.grpc.Channel channel) {
    return new InstrumentServiceBlockingStub(channel);
  }

  /**
   * Creates a new ListenableFuture-style stub that supports unary and streaming output calls on the service
   */
  public static InstrumentServiceFutureStub newFutureStub(
      io.grpc.Channel channel) {
    return new InstrumentServiceFutureStub(channel);
  }

  /**
   */
  public static abstract class InstrumentServiceImplBase implements io.grpc.BindableService {

    /**
     */
    public void getInstruments(krypto.network.rpc.Krypto.Request request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.Instrument> responseObserver) {
      asyncUnimplementedUnaryCall(METHOD_GET_INSTRUMENTS, responseObserver);
    }

    @java.lang.Override public io.grpc.ServerServiceDefinition bindService() {
      return io.grpc.ServerServiceDefinition.builder(getServiceDescriptor())
          .addMethod(
            METHOD_GET_INSTRUMENTS,
            asyncServerStreamingCall(
              new MethodHandlers<
                krypto.network.rpc.Krypto.Request,
                krypto.network.rpc.Krypto.Instrument>(
                  this, METHODID_GET_INSTRUMENTS)))
          .build();
    }
  }

  /**
   */
  public static final class InstrumentServiceStub extends io.grpc.stub.AbstractStub<InstrumentServiceStub> {
    private InstrumentServiceStub(io.grpc.Channel channel) {
      super(channel);
    }

    private InstrumentServiceStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected InstrumentServiceStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new InstrumentServiceStub(channel, callOptions);
    }

    /**
     */
    public void getInstruments(krypto.network.rpc.Krypto.Request request,
        io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.Instrument> responseObserver) {
      asyncServerStreamingCall(
          getChannel().newCall(METHOD_GET_INSTRUMENTS, getCallOptions()), request, responseObserver);
    }
  }

  /**
   */
  public static final class InstrumentServiceBlockingStub extends io.grpc.stub.AbstractStub<InstrumentServiceBlockingStub> {
    private InstrumentServiceBlockingStub(io.grpc.Channel channel) {
      super(channel);
    }

    private InstrumentServiceBlockingStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected InstrumentServiceBlockingStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new InstrumentServiceBlockingStub(channel, callOptions);
    }

    /**
     */
    public java.util.Iterator<krypto.network.rpc.Krypto.Instrument> getInstruments(
        krypto.network.rpc.Krypto.Request request) {
      return blockingServerStreamingCall(
          getChannel(), METHOD_GET_INSTRUMENTS, getCallOptions(), request);
    }
  }

  /**
   */
  public static final class InstrumentServiceFutureStub extends io.grpc.stub.AbstractStub<InstrumentServiceFutureStub> {
    private InstrumentServiceFutureStub(io.grpc.Channel channel) {
      super(channel);
    }

    private InstrumentServiceFutureStub(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      super(channel, callOptions);
    }

    @java.lang.Override
    protected InstrumentServiceFutureStub build(io.grpc.Channel channel,
        io.grpc.CallOptions callOptions) {
      return new InstrumentServiceFutureStub(channel, callOptions);
    }
  }

  private static final int METHODID_GET_INSTRUMENTS = 0;

  private static class MethodHandlers<Req, Resp> implements
      io.grpc.stub.ServerCalls.UnaryMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ServerStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.ClientStreamingMethod<Req, Resp>,
      io.grpc.stub.ServerCalls.BidiStreamingMethod<Req, Resp> {
    private final InstrumentServiceImplBase serviceImpl;
    private final int methodId;

    public MethodHandlers(InstrumentServiceImplBase serviceImpl, int methodId) {
      this.serviceImpl = serviceImpl;
      this.methodId = methodId;
    }

    @java.lang.Override
    @java.lang.SuppressWarnings("unchecked")
    public void invoke(Req request, io.grpc.stub.StreamObserver<Resp> responseObserver) {
      switch (methodId) {
        case METHODID_GET_INSTRUMENTS:
          serviceImpl.getInstruments((krypto.network.rpc.Krypto.Request) request,
              (io.grpc.stub.StreamObserver<krypto.network.rpc.Krypto.Instrument>) responseObserver);
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
        METHOD_GET_INSTRUMENTS);
  }

}
