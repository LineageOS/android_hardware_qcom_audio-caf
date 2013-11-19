ifneq ($(filter msm8960 msm8660 msm8226 msm8x26 msm8974 msm8x74,$(TARGET_BOARD_PLATFORM)),)

ifeq ($(TARGET_QCOM_AUDIO_VARIANT),caf)

MY_LOCAL_PATH := $(call my-dir)

ifeq ($(BOARD_USES_LEGACY_ALSA_AUDIO),true)
    include $(MY_LOCAL_PATH)/legacy/Android.mk
else
    ifeq ($(TARGET_BOARD_PLATFORM),msm8660)
        include $(MY_LOCAL_PATH)/legacy/msm8660/Android.mk
        include $(MY_LOCAL_PATH)/legacy/mm-audio/Android.mk
    else
        include $(MY_LOCAL_PATH)/hal/Android.mk
    endif
endif

endif

endif
