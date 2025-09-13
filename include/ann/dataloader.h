#ifndef DATALOADER_H
#define DATALOADER_H
#include "ann/xtensor_lib.h"
#include "ann/dataset.h"

using namespace std;

template <typename DType, typename LType>
class DataLoader {
 public:
 private:
  Dataset<DType, LType>* ptr_dataset;
  int batch_size;
  bool shuffle;
  bool drop_last;
  //! Add indices, batch, m_seed
  xt::xarray<unsigned long> indices;
  xt::svector<Batch<DType,LType>,sizeof(Batch<DType,LType>)> batch;
  int num_batches;
  int m_seed;
  //! Add indices, batch, m_seed
  // TODO : add more member variables to support the iteration
 public:
  DataLoader(Dataset<DType, LType>* ptr_dataset, int batch_size,
             bool shuffle = true, bool drop_last = false, int seed=-1) {
    // TODO implement
    this->ptr_dataset=ptr_dataset;
    this->batch_size=batch_size;
    this->shuffle=shuffle;
    this->drop_last=drop_last;
    int sample=ptr_dataset->len();
    //!Add seed
    this->m_seed=seed;
    //!Add seed
    indices=xt::arange<int>(sample);
    if(shuffle==true){
      // Use the default random set of numbers
      //xt::random::default_engine_type engine(0);
      // Shuffle the indices along the first dimension (dimension 0)
      if(seed >=0){
        xt::random::seed(m_seed);
        xt::random::shuffle(indices);
      }
      else{
        xt::random::shuffle(indices);
      }
    }
    num_batches=sample/batch_size;
    //if(num_batches==0 && sample%batch_size!=0 && drop_last==false) ++num_batches;
    //batch=XArrayList<Batch<DType,LType>>(0,0,num_batches);
    int startidx=-batch_size;
    int endidx=0;
    for(int i=0;i<num_batches;i++){
        startidx+=batch_size;
        endidx+=batch_size;
        if(i==num_batches-1 && drop_last==false){
          endidx=sample;
        }
        xt::svector<unsigned long> data_shape=ptr_dataset->get_data_shape();
        xt::svector<unsigned long> label_shape=ptr_dataset->get_label_shape();
        xt::svector<size_t> data_sizet(data_shape.begin(), data_shape.end());
        xt::svector<size_t> label_sizet(label_shape.begin(), label_shape.end());
        data_sizet[0]=endidx-startidx;
        label_sizet[0]=endidx-startidx;
        xt::xarray<DType> batch_data(data_sizet);
        xt::xarray<LType> batch_label(label_sizet);
        TensorDataset<DType,LType>* tensor_dataset=dynamic_cast<TensorDataset<DType,LType>*>(ptr_dataset);
        for(int j=startidx;j<endidx;j++){
          DataLabel data_label=tensor_dataset->getitem(indices(j));
          xt::xarray<DType> label_data=data_label.getData();
          xt::xarray<LType> label_label=data_label.getLabel();
          xt::view(batch_data,j-startidx)=label_data;
          if(label_sizet.size()!=0)xt::view(batch_label,j-startidx)=label_label;
        }
        batch.push_back(Batch<DType,LType>(batch_data,batch_label));
    }
  }
  
  virtual ~DataLoader() {
    // TODO implement
    this->ptr_dataset=nullptr;
    this->batch_size=0;
    this->shuffle=false;
    this->drop_last=true;
  }

  // TODO implement forech
  class Iterator {
    private:
    // TODO implement
    DataLoader<DType, LType> *data_loader;
    int cursor;

   public:
    // TODO implement contructor
    Iterator(DataLoader<DType, LType> *data_loader = 0, int cursor = 0) {
      this->data_loader = data_loader;
      this->cursor = cursor;
    }
    //!

    Iterator& operator=(const Iterator& iterator) {
      // TODO implement
      this->data_loader = iterator.data_loader;
      this->cursor = iterator.cursor;
      return *this;
    }

    Iterator& operator++() {
      // TODO implement
      ++cursor;
      return *this;
    }

    Iterator operator++(int) {
      // TODO implement
      Iterator iterator = *this;
      ++*this;
      return iterator;
    }

    bool operator!=(const Iterator& other) const {
      // TODO implement
      return this->cursor != other.cursor;
    }

    Batch<DType, LType> operator*() const {
      // TODO implement
      return data_loader->batch.at(cursor);
    }

  };
  Iterator begin() {
    // TODO implement
    return Iterator(this,0);
  }

  Iterator end() {
    // TODO implement
    return Iterator(this,num_batches);
  }
};

#endif /* DATALOADER_H */