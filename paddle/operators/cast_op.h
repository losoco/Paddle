/* Copyright (c) 2016 PaddlePaddle Authors. All Rights Reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

#pragma once

#include "paddle/framework/data_type.h"
#include "paddle/framework/framework.pb.h"
#include "paddle/framework/op_registry.h"
#include "paddle/platform/transform.h"

namespace paddle {
namespace operators {

template <typename InT, typename OutT>
struct CastOpTransformFunctor {
  HOSTDEVICE OutT operator()(InT in) const { return static_cast<OutT>(in); }
};

template <typename Place, typename InT>
struct CastOpFunctor {
  const framework::Tensor* in_;
  framework::Tensor* out_;
  const platform::DeviceContext& ctx_;
  CastOpFunctor(const framework::Tensor* in, framework::Tensor* out,
                const platform::DeviceContext& ctx)
      : in_(in), out_(out), ctx_(ctx) {}

  template <typename OutT>
  void operator()() const {
    auto* in_begin = in_->data<InT>();
    auto numel = in_->numel();
    auto* in_end = in_begin + numel;
    auto* out_begin = out_->mutable_data<OutT>(ctx_.GetPlace());
    platform::Transform<Place> trans;
    trans(ctx_, in_begin, in_end, out_begin,
          CastOpTransformFunctor<InT, OutT>());
  }
};

template <typename Place, typename InT>
class CastOpKernel : public framework::OpKernel<InT> {
 public:
  void Compute(const framework::ExecutionContext& context) const override {
    auto* in = context.Input<framework::Tensor>("X");
    auto* out = context.Output<framework::Tensor>("Out");
    framework::VisitDataType(
        static_cast<framework::DataType>(context.Attr<int>("out_dtype")),
        CastOpFunctor<Place, InT>(in, out, context.device_context()));
  }
};

}  // namespace operators
}  // namespace paddle
