# gserver pacakge unittests

add_simple_unittest(test_LinearChainCRF)
add_simple_unittest(test_RecurrentLayer)

if(NOT MOBILE_INFERENCE)
  add_simple_unittest(test_MultinomialSampler)
endif()

function(gserver_test TARGET)
  add_unittest_without_exec(${TARGET}
      ${TARGET}.cpp
      LayerGradUtil.cpp)
  add_test(NAME ${TARGET}
      COMMAND ${TARGET})
endfunction()

gserver_test(test_LayerGrad)
gserver_test(test_CRFLayerGrad)
gserver_test(test_CrossEntropyOverBeamGrad)
gserver_test(test_SeqSliceLayerGrad)
gserver_test(test_ActivationGrad)
gserver_test(test_ConvTrans)
gserver_test(test_PriorBox)
gserver_test(test_DetectionOutput)
gserver_test(test_ConvUnify)
gserver_test(test_BatchNorm)
gserver_test(test_KmaxSeqScore)
gserver_test(test_Expand)
gserver_test(test_MaxPoolingWithMaskOutput)

########## test_MKLDNN layers and activations ##########
if(WITH_MKLDNN)
    add_unittest_without_exec(test_MKLDNN
        test_MKLDNN.cpp
        MKLDNNTester.cpp
        LayerGradUtil.cpp)
    add_test(NAME test_MKLDNN
        COMMAND .set_python_path.sh -d ${PADDLE_SOURCE_DIR}/python
            ${CMAKE_CURRENT_BINARY_DIR}/test_MKLDNN
            WORKING_DIRECTORY ${PADDLE_SOURCE_DIR}/paddle)
endif()

############## test_PyDataProvider ########################
if(WITH_PYTHON)
    add_unittest_without_exec(test_PyDataProvider
        test_PyDataProvider.cpp)

    add_test(NAME test_PyDataProvider
        COMMAND .set_python_path.sh -d ./gserver/tests:${PADDLE_SOURCE_DIR}/python/ ${CMAKE_CURRENT_BINARY_DIR}/test_PyDataProvider
        WORKING_DIRECTORY ${PADDLE_SOURCE_DIR}/paddle)
endif()

############### test_WarpCTCLayer #######################
if(NOT WITH_DOUBLE AND NOT MOBILE_INFERENCE)
    add_unittest_without_exec(test_WarpCTCLayer
        test_WarpCTCLayer.cpp)

    add_test(NAME test_WarpCTCLayer
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_WarpCTCLayer --warpctc_dir=${WARPCTC_LIB_DIR}
        WORKING_DIRECTORY ${PADDLE_SOURCE_DIR}/paddle)
endif()

if(NOT MOBILE_INFERENCE)
    ################## test_Evaluator #######################
    add_unittest(test_Evaluator
        test_Evaluator.cpp)
      
    ############### test_RecurrentGradientMachine ###############
    # TODO(yuyang18): There is some bug in test_RecurrentGradientMachine
    # I will fix it.
    add_unittest_without_exec(test_RecurrentGradientMachine
        test_RecurrentGradientMachine.cpp)
    add_test(NAME test_RecurrentGradientMachine
        COMMAND .set_python_path.sh -d
                ${PADDLE_SOURCE_DIR}/python:${PADDLE_SOURCE_DIR}/paddle/gserver/tests
                ${CMAKE_CURRENT_BINARY_DIR}/test_RecurrentGradientMachine
        WORKING_DIRECTORY ${PADDLE_SOURCE_DIR}/paddle)
      
    ############### test_NetworkCompare ###############
    add_unittest_without_exec(test_NetworkCompare
        test_NetworkCompare.cpp)
    if(WITH_GPU)
        add_test(NAME test_NetworkCompare
            COMMAND .set_python_path.sh -d ${PADDLE_SOURCE_DIR}/python ${CMAKE_CURRENT_BINARY_DIR}/test_NetworkCompare --use_gpu=true
            WORKING_DIRECTORY ${PADDLE_SOURCE_DIR}/paddle)
    else()
        add_test(NAME test_NetworkCompare
            COMMAND .set_python_path.sh -d ${PADDLE_SOURCE_DIR}/python ${CMAKE_CURRENT_BINARY_DIR}/test_NetworkCompare --use_gpu=false
            WORKING_DIRECTORY ${PADDLE_SOURCE_DIR}/paddle)
    endif()

    ################# test_CompareSparse ##################
    add_unittest_without_exec(test_CompareSparse
        test_CompareSparse.cpp)
    if(NOT ON_TRAVIS)
      add_test(NAME test_CompareSparse
        COMMAND ${PADDLE_SOURCE_DIR}/paddle/.set_python_path.sh -d
              ${PADDLE_SOURCE_DIR}/python:${PADDLE_SOURCE_DIR}/paddle/gserver/tests
                  ./.set_port.sh -p port -n 6
                      ${CMAKE_CURRENT_BINARY_DIR}/test_CompareSparse
        WORKING_DIRECTORY ${PADDLE_SOURCE_DIR}/paddle/)
    endif()

    ################ test_CompareTwoNets ######################
    add_unittest_without_exec(test_CompareTwoNets
        test_CompareTwoNets.cpp)
    add_test(NAME test_CompareTwoNets
      COMMAND ${PADDLE_SOURCE_DIR}/paddle/.set_python_path.sh -d
            ${PADDLE_SOURCE_DIR}/python:${PADDLE_SOURCE_DIR}/paddle/gserver/tests
            ${CMAKE_CURRENT_BINARY_DIR}/test_CompareTwoNets
        WORKING_DIRECTORY ${PADDLE_SOURCE_DIR}/paddle/)
endif()

################ test_PyDataProvider2 ######################
add_unittest_without_exec(test_PyDataProvider2
        test_PyDataProvider2.cpp)
add_test(NAME test_PyDataProvider2
   COMMAND .set_python_path.sh -d ${PADDLE_SOURCE_DIR}/paddle/gserver/tests:${PADDLE_SOURCE_DIR}/python ${CMAKE_CURRENT_BINARY_DIR}/test_PyDataProvider2
        WORKING_DIRECTORY ${PADDLE_SOURCE_DIR}/paddle
)
