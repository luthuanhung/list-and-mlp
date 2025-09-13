#ifndef DATASET_H
#define DATASET_H
#include "ann/xtensor_lib.h"
using namespace std;

template <typename DType, typename LType>
class DataLabel {
 private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;

 public:
  DataLabel(xt::xarray<DType> data, xt::xarray<LType> label)
      : data(data), label(label) {}
  xt::xarray<DType> getData() const { return data; }
  xt::xarray<LType> getLabel() const { return label; }
};

template <typename DType, typename LType>
class Batch {
 private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;
 public:
  Batch(xt::xarray<DType> data, xt::xarray<LType> label)
      : data(data), label(label) {}
  virtual ~Batch() {}
  xt::xarray<DType>& getData() { return data; }
  xt::xarray<LType>& getLabel() { 
    xt::xarray<LType> nlabel;
    if(label.shape().size()==0){
      label=nlabel;
    }
    return label;
  }
  //! getLabel in class Batch has been added the case label is empty 
  //! Add =,==,<<, constructor with no argument
  friend std::ostream& operator<<(std::ostream& os, const Batch& batch) {
    os <<"( "<<batch.data<<", "<<batch.label<<")";
    return os;
  }

  bool operator==(Batch& other) const {
    return data == other.data && label == other.label;
  }

  Batch& operator=(const Batch& other) {
    // TODO implement
    if(this!=&other){
      this->data=other.data;
      this->label=other.label;
    }
    return *this;
  }
  Batch(){}
  //! Add =,==,<<, constructor with no argument
};

template <typename DType, typename LType>
class Dataset {
 private:
 public:
  Dataset() {};
  virtual ~Dataset() {};

  virtual int len() = 0;
  virtual DataLabel<DType, LType> getitem(int index) = 0;
  virtual xt::svector<unsigned long> get_data_shape() = 0;
  virtual xt::svector<unsigned long> get_label_shape() = 0;
};

//////////////////////////////////////////////////////////////////////
template <typename DType, typename LType>
class TensorDataset : public Dataset<DType, LType> {
 private:
  xt::xarray<DType> data;
  xt::xarray<LType> label;
  xt::svector<unsigned long> data_shape, label_shape;

 public:
  TensorDataset(xt::xarray<DType> data, xt::xarray<LType> label) {
    // TODO implement
    this->data=data;
    this->label=label;
    this->data_shape=xt::svector<unsigned long>(data.shape().begin(),data.shape().end());
    this->label_shape=xt::svector<unsigned long>(label.shape().begin(),label.shape().end());
  }

  int len() {
    // TODO implement
    return this->data_shape[0];
  }

  DataLabel<DType, LType> getitem(int index) {
    // TODO implement
    if(index<0 || index>=this->len()){
      throw std::out_of_range("Index is out of range!");
    }
    xt::xarray<DType> dataidx;
    xt::xarray<LType> labelidx;
    dataidx=xt::view(data,index);
    if(label_shape.size()==0){
      labelidx=label;
    }
    else labelidx=xt::view(label,index);
    return DataLabel(dataidx,labelidx);
  }
  
  xt::svector<unsigned long> get_data_shape() { return data_shape; }
  
  xt::svector<unsigned long> get_label_shape() { return label_shape; }
 
};
#endif /* DATASET_H */
