LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := main_g729Codec.cpp \
    client.cpp \
    codec.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
    $(LOCAL_PATH)/bcg729/include \
    bionic \
    bionic/libstdc++/include \
    external/stlport/stlport \
    frameworks/native/include/utils

LOCAL_SRC_FILES += bcg729/src/LP2LSPConversion.c \
    bcg729/src/LPSynthesisFilter.c \
    bcg729/src/LSPQuantization.c \
    bcg729/src/adaptativeCodebookSearch.c \
    bcg729/src/codebooks.c \
    bcg729/src/computeAdaptativeCodebookGain.c \
    bcg729/src/computeLP.c \
    bcg729/src/computeWeightedSpeech.c \
    bcg729/src/decodeAdaptativeCodeVector.c \
    bcg729/src/decodeFixedCodeVector.c \
    bcg729/src/decodeGains.c \
    bcg729/src/decodeLSP.c \
    bcg729/src/decoder.c \
    bcg729/src/encoder.c \
    bcg729/src/findOpenLoopPitchDelay.c \
    bcg729/src/fixedCodebookSearch.c \
    bcg729/src/gainQuantization.c \
    bcg729/src/interpolateqLSP.c \
    bcg729/src/postFilter.c \
    bcg729/src/postProcessing.c \
    bcg729/src/preProcessing.c \
    bcg729/src/qLSP2LP.c \
    bcg729/src/utils.c \
    bcg729/src/cng.c \
    bcg729/src/vad.c \
    bcg729/src/dtx.c

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/g729Codec/bcg729/include

LOCAL_SHARED_LIBRARIES := libutils libstlport libbinder

LOCAL_MODULE:= g729Codec

include $(BUILD_EXECUTABLE)


