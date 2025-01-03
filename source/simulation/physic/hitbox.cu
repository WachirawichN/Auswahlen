#include "hitbox.cuh"

std::vector<float> sphereHitbox(float position, float radius)
{
    return std::vector<float>({0.0f});
}
std::vector<float> hitbox::cubeHitbox(float position, float scale)
{
    // Return format: {-Axis, Origin, +Axis}
    std::vector<float> borders;
    borders.push_back(position - scale / 2.0f);
    borders.push_back(position);
    borders.push_back(position + scale / 2.0f);

    return borders;
}
std::vector<int> hitbox::hitboxSide(float objPos, float objVel, float tarPos, float tarVel, bool tarInObj, bool objInTar, float timeMultiplier)
{
    // Function will return -1, 1 so its need to be +1 before use
    int objToTarSide = mathExt::aToBside(objPos, tarPos);
    bool isObjFaster = abs(objVel) > abs(tarVel);
    int objVelDirection = mathExt::direction(objVel * timeMultiplier);
    int tarVelDirection = mathExt::direction(tarVel * timeMultiplier);

    // Hollow body
    if (objInTar)
    {
        // Opposite direction
        if (objVelDirection != tarVelDirection)
        {
            if (objVelDirection == mathExt::MIDDLE)
            {
                //std::cout << "Obj velocity is middle" << std::endl;
                return std::vector<int>({tarVelDirection, tarVelDirection});
            }
            //std::cout << "Obj velocity is not middle" << std::endl;
            return std::vector<int>({objVelDirection, objVelDirection});
        }

        // Same direction
        if (objVelDirection != mathExt::MIDDLE)
        {
            //std::cout << "None is middle: " << convertNegative(isObjFaster) << ", " << tarVelDirection << std::endl;
            return std::vector<int>({mathExt::falseToMinus(isObjFaster) * objVelDirection, mathExt::falseToMinus(isObjFaster) * objVelDirection});
        }
    }
    else if (tarInObj)
    {
        // Opposite direction
        if (objVelDirection != tarVelDirection)
        {
            if (tarVelDirection == mathExt::MIDDLE)
            {
                //std::cout << "Tar velocity is middle" << std::endl;
                return std::vector<int>({objVelDirection, objVelDirection});
            }
            //std::cout << "Tar velocity is not middle" << std::endl;
            return std::vector<int>({tarVelDirection, tarVelDirection});
        }

        // Same direction
        if (tarVelDirection != mathExt::MIDDLE)
        {
            //std::cout << "None is middle: " << convertNegative(isObjFaster) << ", " << tarVelDirection << std::endl;
            return std::vector<int>({mathExt::falseToMinus(!isObjFaster) * tarVelDirection, mathExt::falseToMinus(!isObjFaster) * tarVelDirection});
        }
    }

    // Solid body
    //std::cout << "Solid body" << std::endl;
    return std::vector<int>({objToTarSide, objToTarSide * -1});
}