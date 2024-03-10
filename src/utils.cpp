#include "HOMHT/utils.h"

void removeRow(Eigen::MatrixXi &matrix, Eigen::Index rowToRemove)
{
    Eigen::Index numRows = matrix.rows() - 1;
    Eigen::Index numCols = matrix.cols();

    if (rowToRemove < numRows)
        matrix.block(rowToRemove, 0, numRows - rowToRemove, numCols) =
          matrix.block(rowToRemove + 1, 0, numRows - rowToRemove, numCols);

    matrix.conservativeResize(numRows, numCols);
}

void removeColumn(Eigen::MatrixXi &matrix, Eigen::Index colToRemove)
{
    Eigen::Index numRows = matrix.rows();
    Eigen::Index numCols = matrix.cols() - 1;

    if (colToRemove < numCols)
        matrix.block(0, colToRemove, numRows, numCols - colToRemove) =
          matrix.block(0, colToRemove + 1, numRows, numCols - colToRemove);

    matrix.conservativeResize(numRows, numCols);
}
