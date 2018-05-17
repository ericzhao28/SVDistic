#include "./svdpp/model.h"
#include "./utils/pipes.h"


int infer()
{
  ExampleMat X_test;
  cout << "Loading testing data..." << endl;
  if (input_pipeline(X_test, "data/to_infer.txt") == 1)
  {
    cout << "Invalid number of testing examples." << endl;
    return 1;
  }
  cout << "Testing data loaded." << endl;

  UserMat W_u;
  UserMat Ysum_terms;
  ProductMat W_i;
  ProductMat W_p;
  UserVec Ru_terms;
  cout << "Loading weights..." << endl;
  load_weights(W_u, W_i, W_p, Ru_terms, Ysum_terms, "weights/W_u.txt",
               "weights/W_i.txt", "weights/W_p.txt", "weights/Ru.txt",
               "weights/Ysum.txt");
  cout << "Loading weights." << endl;

  cout << "Starting inference..." << endl;
  ExampleVec Pred_terms;
  calc_Pred_terms(Pred_terms, X_test, W_u, W_p,
                  Ysum_terms, Ru_terms);
  cout << "Inference completed." << endl;

  cout << "Saving inferences..." << endl;
  save_pipeline(Pred_terms, "data/inferred.txt");
  cout << "Inferences saved." << endl;

  return 0;
}


int score()
{
  ExampleMat X_val;
  cout << "Loading validation data..." << endl;
  if (input_pipeline(X_val, "data/validation.txt") == 1)
  {
    cout << "Invalid number of validation examples." << endl;
    return 1;
  }
  cout << "Validation data loaded." << endl;

  UserMat W_u;
  UserMat Ysum_terms;
  ProductMat W_i;
  ProductMat W_p;
  UserVec Ru_terms;
  cout << "Loading weights..." << endl;
  load_weights(W_u, W_i, W_p, Ru_terms, Ysum_terms, "weights/W_u.txt",
               "weights/W_i.txt", "weights/W_p.txt", "weights/Ru.txt",
               "weights/Ysum.txt");
  cout << "Loading weights." << endl;

  cout << "Starting inference..." << endl;
  ExampleVec Err_terms;
  calc_Err_terms(Err_terms, X_val, W_u, W_p, Ysum_terms, Ru_terms);
  cout << "Inference completed." << endl;

  cout << "RMSE: " << Err_terms.squaredNorm() << endl;

  return 0;
}

int train()
{
  ExampleMat X_tr;
  cout << "Loading training data..." << endl;
  if (input_pipeline(X_tr, "data/train.txt") == 1)
  {
    cout << "Invalid number of training examples." << endl;
    return 1;
  }
  cout << "Training data loaded." << endl;

  UserMat W_u;
  UserMat Ysum_terms = UserMat::Zero(N_LATENT, N_USER);
  ProductMat W_i;
  ProductMat W_p;
  cout << "Initializing weights..." << endl;
  init_weights(W_u, W_i, W_p);
  cout << "Initialized weights." << endl;

  cout << "Initializing RU_terms..." << endl;
  UserVec Ru_terms = UserVec::Zero();
  calc_Ru_terms(Ru_terms, X_tr);
  cout << "RU_norm initialized." << endl;

  cout << "Starting training..." << endl;
  for (int epoch = 0; epoch < N_EPOCHS; ++epoch)
  {
    cout << "Epoch " << epoch << " starting..." << endl;
    train(X_tr, W_u, W_i, W_p, Ru_terms, Ysum_terms);
  }
  cout << "Training completed." << endl;

  cout << "Saving weights..." << endl;
  save_pipeline(W_u, W_i, W_p, Ru_terms, Ysum_terms, "weights/W_u.txt",
               "weights/W_i.txt", "weights/W_p.txt", "weights/Ru.txt",
               "weights/Ysum.txt");
  cout << "Weights saved." << endl;

  return 0;
}


int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    cout << "Usage: ./train_svdpp [-train/-infer/-score]" << endl;
    return 1;
  }

  if (!strcmp(argv[1], "-train"))
    return train();
  if (!strcmp(argv[1], "-infer"))
    return infer();
  if (!strcmp(argv[1], "-score"))
    return score();

  cout << "Usage: ./train_svdpp [-train/-infer/-score]" << endl;
  return 1;
}

