#ifndef BALL_LINALG_ROWITERATOR_H
#define BALL_LINALG_ROWITERATOR_H

#ifndef BALL_LINALG_MATRIX_IH
# include <BALL/MATHS/LINALG/matrix.ih>
#endif

#ifndef BALL_LINALG_VECTOR_IH
# include <BALL/MATHS/LINALG/vector.ih>
#endif

#ifndef BALL_CONCEPT_RANDOMACCESSITERATOR_H
#include <BALL/CONCEPT/randomAccessIterator.h>
#endif


typedef unsigned int uint;

namespace BALL {

	// forward declaration
	template <class valuetype, class mtraits>
	class Matrix;

	template <class valuetype, class mtraits=StandardTraits>
	class RowIteratorTraits
	 {

		/**
		*/
		typedef valuetype ValueType;

		/**
		*/
		typedef valuetype* PointerType;

		/**
		*/
		typedef int IteratorPosition;

		/**
		*/
		typedef int Distance;

		/**
		*/
		typedef int Index;

		friend class Matrix<valuetype, mtraits>;
	  public:

			virtual ~RowIteratorTraits()
			{
			}

			RowIteratorTraits()
				:	bound_(0),
					position_(0),
					vector_(0)
			{
			}
			
			RowIteratorTraits(const Matrix<valuetype, mtraits>& matrix)
				:	bound_(const_cast<Matrix<valuetype, mtraits>*>(&matrix)),
					position_(0),
					vector_(bound_->m_)
			{
			}
			
			RowIteratorTraits(const RowIteratorTraits& traits)
				:	bound_(traits.bound_),
					position_(traits.position_),
					vector_(bound_->m_)
			{
			}
			
			RowIteratorTraits& operator = (const RowIteratorTraits& traits)
			{
				bound_ = traits.bound_;
				position_ = traits.position_;
				vector_ = traits.vector_;
		
				return *this;
			}

			Matrix<valuetype, mtraits>* getContainer()
			{
				return bound_;
			}
			
			const Matrix<valuetype, mtraits>* getContainer() const
			{
				return bound_;
			}
			
			bool isSingular() const
			{
				return (bound_ == 0);
			}
			
			IteratorPosition& getPosition()
			{
				return position_;
			}

			const IteratorPosition& getPosition() const
			{
				return position_;
			}

			bool operator == (const RowIteratorTraits& traits) const
			{
			  return (position_ == traits.position_);
			}

			bool operator != (const RowIteratorTraits& traits) const
			{
				return (position_ != traits.position_);
			}
				
			bool operator < (const RowIteratorTraits& traits) const
			{
			  return (position_ < traits.position_);
			}

			Distance getDistance(const RowIteratorTraits& traits) const
			{
			  return (Distance)(position_ - traits.position_);
			}
			
			bool isValid() const
			{
			  return ((bound_ != 0) && (position_ >= 0) && (position_ < (int)bound_->data_.size()));
			}

			void invalidate()
			{
				bound_ = 0;
				position_ = -1;
			}
			
			void toBegin()
			{
			  position_ = 0;
			}

			bool isBegin() const
			{
			  return ( position_ == 0 );
			}

			void toEnd()
			{
			  position_ = bound_->data_.size();
			}
			
			bool isEnd() const
			{
			  return ( position_ == (int)bound_->data_.size());
			}
			
			Vector<valuetype>& getData()
			{

			  if (bound_->row_major_)
			  {
			    for (uint i = 0; i < bound_->m_; i++)
			    {
			      vector_[i]=&(*bound_)[position_+i];
			    }
			  }
			  else
			  {
			    uint j = 0;
			    for (uint i = 0; i < bound_->data_.size(); i+=bound_->n_)
			    {
			      vector_[j++]=&(*bound_)[position_+i];
			    }
			  }

			  return vector_;
			}

			const Vector<valuetype>& getData() const
			{

			  if (bound_->row_major_)
			  {
			    for (uint i = 0; i < bound_->m_; i++)
			    {
			      vector_[i]=(*bound_)[position_+i];
			    }
			  }
			  else
			  {
			    uint j = 0;
			    for (uint i = 0; i < bound_->data_.size(); i+=bound_->n_)
			    {
			      vector_[j++]=(*bound_)[position_+i];
			    }
			  }
			  return vector_;

			}

			void forward()
			{
			  if (bound_->row_major_)
			  {
			    position_ += bound_->m_;
			  }
			  else
			  {
			    position_++;
			    if (position_ == (int)bound_->n_)
			      position_ = bound_->data_.size();
			  }
			}

			friend std::ostream& operator << (std::ostream& s, const RowIteratorTraits& traits)
			{
			  return (s << traits.position_ << ' ');
			}
			
			void dump(std::ostream& s) const
			{
			  s << position_ << std::endl;
			}
			
			void toRBegin()
			{
			  position_ = bound_->data_.size() - 1;
			}
			
			bool isRBegin() const
			{
			  return (position_ == bound_->data_.size() - 1);
			}
			
			void toREnd()
			{
			  position_ = -1;
			}

			bool isREnd() const
			{
			  return (position_ <= -1);
			}
			
			void backward()
			{
			  if (bound_->row_major_)
			  {
			    if (position_ == 0)
			      position_--;
			    else
			      position_ -= bound_->m_;
			  }
			  else
			  {
			    if (position_ == (int)bound_->data_.size())
			      position_ = bound_->n_;
			    position_--;
			  }
			}

			void backward(Distance distance)
			{			  
			  if (bound_->row_major_)
			  {
			    if (position_-(distance * (int)bound_->m_) < 0)
			    {
			      position_ = -1;
			      return;
			    }
			    position_ -= (distance * (int)bound_->m_);
			    
			  }
			  else
			  {
			    if (position_ == (int)bound_->data_.size())
			      position_ = bound_->n_;
			    if (position_-distance < 0)
			    {
			      position_ = -1;
			      return;
			    }
			    position_ -= distance;
			  }
			}

			void forward(Distance distance)
			{
			  
			  if (bound_->row_major_)
			  {
			    if (position_+(distance * bound_->m_) > bound_->data_.size())
			    {
			      position_ = bound_->data_.size();
			      return;
			    }
			    position_ += (distance * bound_->m_);
			  }
			  else
			  {
			    position_ += distance;
			    if (position_ >= (int)bound_->n_)
			      position_ = bound_->data_.size();
			  }
			}
			
			Vector<valuetype>& getData(Index index)
			{

			  if (bound_->row_major_)
			  {
			    for (uint i = 0; i < bound_->m_; i++)
			      vector_[i]=(*bound_)[index+i];
			  }
			  else
			  {
			    for (uint i = 0; i < bound_->m_; i+=bound_->n_)
			      vector_[i]=(*bound_)[index+i];
			  }

			  return vector_;
			}
			
			const Vector<valuetype>& getData(Index index) const
			{

			  if (bound_->row_major_)
			  {
			    for (uint i = 0; i < bound_->m_; i++)
			      vector_[i]=(*bound_)[index+i];
			  }
			  else
			  {
			    for (uint i = 0; i < bound_->m_; i+=bound_->n_)
			      vector_[i]=(*bound_)[index+i];
			  }

			  return vector_;
			}
			

			protected:

			Matrix<valuetype, mtraits>*		bound_;
			IteratorPosition		position_;
			mutable Vector<valuetype>           vector_;
		};


} // namespace BALL

#endif