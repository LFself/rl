//
// Copyright (c) 2009, Markus Rickert
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#include <rl/math/Rotation.h>

#include "Spherical.h"

namespace rl
{
	namespace mdl
	{
		Spherical::Spherical() :
			Joint(4, 3)
		{
			this->qUnits(0) = ::rl::math::UNIT_NONE; // TODO
			this->qUnits(1) = ::rl::math::UNIT_NONE; // TODO
			this->qUnits(2) = ::rl::math::UNIT_NONE; // TODO
			this->qUnits(3) = ::rl::math::UNIT_NONE; // TODO
			this->qdUnits(0) = ::rl::math::UNIT_RADIAN_PER_SECOND;
			this->qdUnits(1) = ::rl::math::UNIT_RADIAN_PER_SECOND;
			this->qdUnits(2) = ::rl::math::UNIT_RADIAN_PER_SECOND;
			this->qddUnits(0) = ::rl::math::UNIT_RADIAN_PER_SECOND_SQUARED;
			this->qddUnits(1) = ::rl::math::UNIT_RADIAN_PER_SECOND_SQUARED;
			this->qddUnits(2) = ::rl::math::UNIT_RADIAN_PER_SECOND_SQUARED;
			this->S(0, 0) = 1;
			this->S(1, 1) = 1;
			this->S(2, 2) = 1;
			this->speedUnits(0) = ::rl::math::UNIT_RADIAN_PER_SECOND;
			this->speedUnits(1) = ::rl::math::UNIT_RADIAN_PER_SECOND;
			this->speedUnits(2) = ::rl::math::UNIT_RADIAN_PER_SECOND;
			this->tauUnits(0) = ::rl::math::UNIT_NEWTON_METER;
			this->tauUnits(1) = ::rl::math::UNIT_NEWTON_METER;
			this->tauUnits(2) = ::rl::math::UNIT_NEWTON_METER;
		}
		
		Spherical::~Spherical()
		{
		}
		
		void
		Spherical::clip(::rl::math::Vector& q) const
		{
			::Eigen::Map< ::rl::math::Quaternion>(q.data()).normalize();
		}
		
		::rl::math::Real
		Spherical::distance(const ::rl::math::Vector& q1, const ::rl::math::Vector& q2) const
		{
			::Eigen::Map<const ::rl::math::Quaternion> quaternion1(q1.data());
			::Eigen::Map<const ::rl::math::Quaternion> quaternion2(q2.data());
			return quaternion1.angularDistance(quaternion2);
		}
		
		::rl::math::Vector
		Spherical::generatePositionGaussian(const ::rl::math::ConstVectorRef& rand, const ::rl::math::ConstVectorRef& mean, const ::rl::math::ConstVectorRef& sigma) const
		{
			::rl::math::Quaternion quaternion;
			quaternion.setFromGaussian(rand, ::Eigen::Map< const ::rl::math::Quaternion>(mean.data()), sigma);
			return quaternion.coeffs();
		}
		
		::rl::math::Vector
		Spherical::generatePositionUniform(const ::rl::math::ConstVectorRef& rand) const
		{
			::rl::math::Quaternion quaternion;
			quaternion.setFromUniform(rand);
			return quaternion.coeffs();
		}
		
		void
		Spherical::interpolate(const ::rl::math::Vector& q1, const ::rl::math::Vector& q2, const ::rl::math::Real& alpha, ::rl::math::Vector& q) const
		{
			::Eigen::Map<const ::rl::math::Quaternion> quaternion1(q1.data());
			::Eigen::Map<const ::rl::math::Quaternion> quaternion2(q2.data());
			q = quaternion1.slerp(alpha, quaternion2).coeffs();
		}
		
		void
		Spherical::normalize(::rl::math::Vector& q) const
		{
			::Eigen::Map< ::rl::math::Quaternion> quaternion(q.data());
			
			if (quaternion.squaredNorm() > 0)
			{
				quaternion.normalize();
			}
			else
			{
				quaternion.setIdentity();
			}
		}
		
		void
		Spherical::setPosition(const ::rl::math::Vector& q)
		{
			this->q = q;
			this->t = ::Eigen::Map<const ::rl::math::Quaternion>(this->q.data());
			this->x.rotation() = this->t.linear().transpose();
		}
		
		void
		Spherical::step(const ::rl::math::Vector& q1, const ::rl::math::Vector& qdot, ::rl::math::Vector& q2) const
		{
			::Eigen::Map<const ::rl::math::Quaternion> quaternion1(q1.data());
			q2 = (quaternion1 * ::rl::math::AngleAxis(qdot.norm(), qdot.normalized())).coeffs();
		}
		
		::rl::math::Real
		Spherical::transformedDistance(const ::rl::math::Vector& q1, const ::rl::math::Vector& q2) const
		{
			return ::std::pow(this->distance(q1, q2), 2);
		}
	}
}
