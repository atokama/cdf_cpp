#include <cdf_cpp/variable.h>

namespace cdf_cpp {

    Variable::Variable(CDFid id, long varnum)
            : _id{id}, _varnum{varnum} {
        check_status(CDFgetzVarNumRecsWritten(_id, _varnum, &num_elements),
                     "fail to get number of variable elements");
        check_status(CDFgetzVarDataType(_id, _varnum, &_datatype),
                     "fail to get datatype of variable");

        long  varNum, numDims, recVary,
              dimIndices[CDF_MAX_DIMS], dimCounts[CDF_MAX_DIMS],
              dimInterval[CDF_MAX_DIMS], dimSizes[CDF_MAX_DIMS], dimVarys[CDF_MAX_DIMS];
        long  datatype, numElements, numRecs, arraySize;
        long  ilen;

        auto REQUIRE = [](bool b) -> void {
            if (!b) throw CDFError{"REQUIRE"};
        };

        /******************************************************************************
        * Read data for 'Image' zVariable (10 x 20 array).
        ******************************************************************************/
        dimIndices[0] = 0;
        dimIndices[1] = 0;
        dimInterval[0] = 1;
        dimInterval[1] = 1;

        //varNum = CDFgetVarNum (id, "HEZ");
        varNum = varnum;
        if (varNum < CDF_OK) REQUIRE(false);

        char varName[CDF_VAR_NAME_LEN];
        varName[0] = '\0';
        CDFstatus status = CDFinquirezVar (
                id, varNum, varName, &datatype,
                &numElements, &numDims, dimSizes, &recVary, dimVarys);
        if (status != CDF_OK) REQUIRE(false);

        status = CDFgetzVarNumRecsWritten (id, varNum, &numRecs);
        if (status != CDF_OK) REQUIRE(false);

        status = CDFgetDataTypeSize (datatype, &ilen);
        ilen = ilen * numElements;

        arraySize = 1;
        for (auto i=0; i < numDims; i++) {
             arraySize *= dimSizes[i];
             dimCounts[i] = dimSizes[i];
        }
        _num_columns = arraySize;
        _num_rows = numRecs;
        _image.resize(ilen * arraySize * numRecs);

        /* read 3 records in one scoop */
        status = CDFhyperGetzVarData (id, varNum,
                                      0L,           /* record start */
                                      numRecs,           /* # of records to read */
                                      1L,           /* record interval */
                                      dimIndices,   /* dimension dimIndices */
                                      dimCounts,    /* dimension counts */
                                      dimInterval,  /* dimension interval */
                                      _image.data());
        if (status != CDF_OK) REQUIRE(false);
    }

    Variable::~Variable() {
    }
}
