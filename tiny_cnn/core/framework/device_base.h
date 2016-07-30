/*
    Copyright (c) 2016, Taiga Nomi, Edgar Riba
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#pragma once

#include "tiny_cnn/layers/layer.h"

#ifdef USE_OPENCL
#include "third_party/CLCudaAPI/clpp11.h"
#endif

namespace tiny_cnn {

/* Supported devices type
 *
 * */
enum device_t { CPU, GPU, FPGA };

/* Base class modeling a device 
 *
 * @param type The type of the device
 * @param id The identification number
 *
 * */
class device_base {
 public:
    explicit device_base(const device_t type, const int id)
        : id_(id)
        , type_(type)
        , platform_(CLCudaAPI::Platform(size_t{0}))
        , device_(CLCudaAPI::Device(platform_, id_))
        , context_(CLCudaAPI::Context(device_))
        , queue_(CLCudaAPI::Queue(context_, device_)) {}

    // Register an ops to the current device
    void register_op(const std::vector<layer*>& ops) {
        for (auto o: ops) {
            ops_.push_back(o);
            // o->set_device(this);
        }
    }

    // Inits the device context
    void init() {

    }

    // Returns the device type
    device_t type() const { return type_; }

    // Returns the device id
    int id() const { return id_; }

 private:
    /* The id of the current device */
    int id_;

    /* The type of the device */
    device_t type_;

#ifdef USE_OPENCL
    CLCudaAPI::Platform platform_;
    CLCudaAPI::Device device_;
    CLCudaAPI::Context context_;
    CLCudaAPI::Queue queue_;

#endif  // USE_OPENCL

    /* A vector of pointers to registered ops.
     * The data is not owned by the current class.
     * */
    std::vector<layer*> ops_;
};

/* Public interface for a CPU device
 *
 * @param id The identification number
 *
 * */
class cpu_device : public device_base {
 public:
    explicit cpu_device(const int id)
        : device_base(device_t::CPU, id) {}
};

/* Public interface for a GPU device
 *
 * @param id The identification number
 *
 * */
class gpu_device : public device_base {
 public:
    explicit gpu_device(const int id)
        : device_base(device_t::GPU, id) {}
};

}  // namespace tiny_cnn
