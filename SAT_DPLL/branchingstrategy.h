#ifndef BRANCHINGSTRATEGY_H
#define BRANCHINGSTRATEGY_H

class BoolEquation;

class BranchingStrategy
{
public:
	virtual ~BranchingStrategy() = default;
    virtual int ChooseColumn(BoolEquation &equation) const = 0;
};



class MostContraintBranchingStrategy : public BranchingStrategy
{
public:
    int ChooseColumn(BoolEquation &equation) const override;
};

#endif // BRANCHINGSTRATEGY_H
