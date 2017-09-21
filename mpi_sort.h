#include <string>
#include  <mpi.h>
#include <algorithm>
#include <math.h>


struct SortByLength
{
  bool inline operator()(const std::string& a, const std::string& b) throw()
  {
    //If the lengths are equal, compare normally
    if(a.length() == b.length()){
      return a < b;
    }
    //else compare lengths
    return a.length() < b.length();
  }
};


template <typename DataType, typename Func>
std::vector<DataType> merge(std::vector<DataType> vec1, std::vector<DataType> vec2,
                            const Func& func)
{
  std::vector<DataType> result_vector;
  std::merge(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result_vector.begin(), func);
  retrun result_vector;
}


template <typename DataType, typename Func>
void MPI_Sort(std::vector<DataType> &data, std::size_t num_proc,
              const Func& func = SortByLength{}, MPI_Comm comm, int* rank)
{
  std::size_t data_size = data.size()/num_proc;
  if(data_size > 500)
    std::sort(data.begin(), data.end(), func);
  else
  {
    std::vector<DataType> sub_data(data_size);

    MPI_Scatter(&data[i], data_size, MPI::INT, &sub_data[0], data_size,
                MPI_INT, 0, comm);

    std::sort(sub_data.begin(), sub_data.end(), func);
    MPI_Barrier(comm);

    std::size_t level = 1;
    for (std::size_t i = pow(2,level); i <= num_proc; i = pow(2,level))
    {
      std::vector<DataType> recv_data(data_size * level);
      if(rank % (i/2) == 0)
      {
        if(rank % i == 0)
          MPI_Recv(&recv_data[0], data_size, MPI_INT, rank, 0, comm, MPI_STATUS_IGNORE);
        else
          MPI_Send(&sub_data[0], data_size, MPI_INT, rank-1, 0, comm);
      }
      std::vector<DataType> result = merge(sub_data, recv_data);
      sub_data.clear();
      sub_data.resize(result.begin(), result.end());
      MPI_Barrier(comm);
      level++;
    }

  }
}
