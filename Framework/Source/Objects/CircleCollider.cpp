#include "Framework.h"
#include "CircleCollider.h"

namespace fw {

    CircleCollider::CircleCollider(fw::GameObject* _pparent, float _radius, Vector2 _relativePos, bool _resolveCollisions):
        Collider(_pparent), m_Radius(_radius)
    {
        m_RelativePosition = _relativePos;
        m_ResolveCollisions = _resolveCollisions;
        m_pDebugMesh = new Mesh();
        m_pDebugMesh->MakeCircle(m_Radius, 18, false);
    }

    CircleCollider::CircleCollider(fw::GameObject* _pparent, float _radius, Camera* _pcamera, ShaderProgram* _pdebugshader, Vector2 _relativePos, bool _resolveCollisions): 
        Collider(_pparent, _pcamera, _pdebugshader), m_Radius(_radius)
    {
        m_RelativePosition = _relativePos;
        m_ResolveCollisions = _resolveCollisions;

        m_pDebugMesh = new Mesh();
        m_pDebugMesh->MakeCircle(m_Radius, 18, false);
    }

    CircleCollider::CircleCollider(): Collider(), m_Radius(0)
    {
        m_pDebugMesh = new Mesh();
    }

    CircleCollider::~CircleCollider(){
        delete m_pDebugMesh;
    }

    float CircleCollider::GetRadius()
    {
        return m_Radius;
    }

    void CircleCollider::SetRadius(float _r)
    {
        m_Radius = _r;
        m_pDebugMesh->MakeCircle(m_Radius, 18, false);
    }

    k_ColliderType CircleCollider::GetType()
    {
        return k_ColliderType::Circle;
    }

    bool CircleCollider::CheckCollision(Collider* _pOtherCollider)
    {
        if (!m_Enabled)
            return false;

        if (_pOtherCollider->GetType() == k_ColliderType::Circle)
        {
            float otherRadius = (static_cast<CircleCollider*>(_pOtherCollider))->GetRadius();
            float sqrDistance = (GetRelativePosition() - _pOtherCollider->GetRelativePosition()).SqrMagnitude(); // if distance between two parents is more than the sum of radii
            bool overlap = sqrDistance < (m_Radius + otherRadius)* (m_Radius + otherRadius);

            unsigned char WhoseCollisionToResolve = GetResolveCollisions() + _pOtherCollider->GetResolveCollisions();

            switch (WhoseCollisionToResolve)
            {
            case 0:
                return overlap;
                break;

            case 1:
                if (overlap) 
                {
                    if (GetResolveCollisions()) // other circle is static
                    {
                        m_pParent->SetPosition(_pOtherCollider->GetRelativePosition() + (GetRelativePosition()- _pOtherCollider->GetRelativePosition()).Normalized() * (m_Radius + otherRadius));
                    }
                    else                        // this circle is static
                    {
                        _pOtherCollider->GetParent()->SetPosition(GetRelativePosition() + (_pOtherCollider->GetRelativePosition() - GetRelativePosition()).Normalized() * (m_Radius + otherRadius));
                    }
                }
                return overlap;
                break;

            case 2:                             // both circles are dynamic
                if (overlap)
                {
                    float radiusFraction = otherRadius / (otherRadius + m_Radius);
                    Vector2 displacement = (GetRelativePosition() - _pOtherCollider->GetRelativePosition()).Normalized() * ((m_Radius + otherRadius) - sqrt(sqrDistance));
                    m_pParent->SetPosition(m_pParent->GetPosition() + displacement * radiusFraction);
                    _pOtherCollider->GetParent()->SetPosition(_pOtherCollider->GetParent()->GetPosition() - displacement * (1-radiusFraction));
                }
                return overlap;
                break;

            default:
                /// UNEXPECTED INT
                assert(false); 
                break;
            }

            return (overlap);
        }


        if (_pOtherCollider->GetType() == k_ColliderType::Rectangle)
        {
            bool overlap = false;
            unsigned char ULDRflag = 0;
            RectangleCollider* rectCollider = static_cast<RectangleCollider*>(_pOtherCollider);;
            CircleCollider* circleCollider = this;

            Vector2 rectPos = rectCollider->GetRelativePosition();
            Vector2 rectExtents = rectCollider->m_Extents;

            Vector2 rectCorners[4] = {
                 rectPos + rectExtents * 0.5f,
                 rectPos + Vector2(rectExtents.x, -rectExtents.y) * 0.5f,
                 rectPos - rectExtents * 0.5f,
                 rectPos + Vector2(-rectExtents.x, rectExtents.y) * 0.5f,
            };

            Vector2 circlePos = circleCollider->GetRelativePosition();
            Vector2 PosDiff = circlePos - rectPos;

            float circleRadius = circleCollider->GetRadius();
            float squareDistance = (PosDiff).SqrMagnitude();
            int ClosestCorner = 0;

            //Check outer circle
            if (squareDistance > (rectCollider->m_rectOuterCircleRadius + circleRadius) * (rectCollider->m_rectOuterCircleRadius + circleRadius))
            {
                //ImGui::Text("OuterCircle, too far");
                return false;
            }
            //Check inner circle
            else if (squareDistance < (rectCollider->m_rectInnerCircleRadius + circleRadius) * (rectCollider->m_rectInnerCircleRadius + circleRadius))
            {
                //HandleCollision
                //ImGui::Text("Inner circle, too close");
                overlap = true;
            }
            //Check edge cases
            else
            {
                // order is top right, clockwise

                float CornerSqrDistances[4]{};

                for (char i = 0; i < 4; i++)
                {
                    CornerSqrDistances[i] = rectCorners[i].SqrDistanceFrom(circlePos);
                    if (rectCorners[ClosestCorner].SqrDistanceFrom(circlePos) > CornerSqrDistances[i])
                        ClosestCorner = i;
                }
                //ImGui::Text("closest corner %d", ClosestCorner);

                if (CornerSqrDistances[ClosestCorner] < circleRadius * circleRadius || abs(PosDiff.x) < circleRadius + rectExtents.x * 0.5f || abs(PosDiff.y) < circleRadius + rectExtents.y * 0.5f)
                    overlap = true;
            }

            //overlap ?
            //    ImGui::Text("Yep") :
            //    ImGui::Text("Nop");

            // return if no overlap.
            if (!overlap)
                return false;
            // who needs resolving?
            else
            {
                unsigned char WhoseCollisionToResolve = GetResolveCollisions() + _pOtherCollider->GetResolveCollisions();
                bool WithinXRange = abs(PosDiff.x) < rectExtents.x * 0.5f;
                bool WithinYRange = abs(PosDiff.y) < rectExtents.y * 0.5f;

                switch (WhoseCollisionToResolve)
                {
                    // nobody needs resolving.
                case 0:
                    return true;
                    break;

                    // one of them needs resolving.
                case 1:
                    // Circle is static.
                    if (rectCollider->GetResolveCollisions())
                    {
                        //ImGui::Text("overlap amt = %f", (circlePos - rectCorners[ClosestCorner]).Magnitude());
                        if (WithinXRange)
                        {
                            // CIRCLE IS INSIDE
                            if (WithinYRange)
                            {
                                // must be pushed in y
                                if (abs(PosDiff.x) < abs(PosDiff.y))
                                {
                                    rectCollider->GetParent()->SetPosition(rectPos.x, circlePos.y - (rectExtents.y * 0.5f + circleRadius) * (circlePos.y > rectPos.y ? 1 : -1));
                                }
                                // must be pushed in x
                                else
                                {
                                    rectCollider->GetParent()->SetPosition(circlePos.x - (rectExtents.x * 0.5f + circleRadius) * (circlePos.x > rectPos.x ? 1 : -1), rectPos.y);
                                }

                                return true;
                            }
                            // X in range, Y may or may not be poking in.
                            else
                            {
                                // X in range, Y poking in
                                if (abs(circlePos.y - rectPos.y) < circleRadius + rectExtents.y * 0.5f)
                                {
                                    //ImGui::Text("X in range, Y poking in");
                                    rectCollider->GetParent()->SetPosition(rectPos.x, circlePos.y - (rectExtents.y * 0.5f + circleRadius) * (circlePos.y > rectPos.y ? 1 : -1));
                                    return true;
                                }
                                // X in range, Y not poking in
                                else
                                    return false;
                            }
                        }
                        else
                        {   // Y in range, X may or may not be poking in.
                            if (WithinYRange)
                            {
                                // Y in range, X poking in.
                                if (abs(circlePos.x - rectPos.x) < circleRadius + rectExtents.x * 0.5f)
                                {
                                    //ImGui::Text("Y in range, X poking in");
                                    rectCollider->GetParent()->SetPosition(circlePos.x - (rectExtents.x * 0.5f + circleRadius) * (circlePos.x > rectPos.x ? 1 : -1), rectPos.y);
                                    return true;
                                }
                                // Y in range, X not poking in.
                                else
                                    return false;
                            }
                            else
                            {
                                //CORNER CASE
                                Vector2 cornerToCenter = (rectPos - rectCorners[ClosestCorner]);
                                Vector2 circleToEdgeOfCircle = (rectCorners[ClosestCorner] - circlePos).Normalized() * circleRadius;
                                rectCollider->GetParent()->SetPosition(circlePos + circleToEdgeOfCircle + cornerToCenter);
                                return true;
                            }
                        }
                        //if (!WithinXRange && !WithinYRange) // Corner case
                        //    rectCollider->GetParent()->SetPosition(rectCollider->GetParent()->GetPosition() + (rectCorners[ClosestCorner] - circlePos).Normalized() * (circleRadius - (circlePos - rectCorners[ClosestCorner]).Magnitude()));
                    }
                    // Rectangle is static.
                    // TODO: FIX PARENT RELATIVE LOCATION
                    else
                    {
                        if (WithinXRange)
                        {
                            // CIRCLE IS INSIDE
                            if (WithinYRange)
                            {
                                // must be pushed in y
                                if (abs(PosDiff.x) < abs(PosDiff.y))
                                {
                                    circleCollider->GetParent()->SetPosition(circlePos.x, rectPos.y + (rectExtents.y * 0.5f + circleRadius) * (circlePos.y > rectPos.y ? 1 : -1));
                                }
                                // must be pushed in x
                                else
                                {
                                    circleCollider->GetParent()->SetPosition(rectPos.x + (rectExtents.x * 0.5f + circleRadius) * (circlePos.x > rectPos.x ? 1 : -1), circlePos.y);
                                }

                                return true;
                            }
                            // X in range, Y may or may not be poking in.
                            else
                            {
                                // X in range, Y poking in
                                if (abs(circlePos.y - rectPos.y) < circleRadius + rectExtents.y * 0.5f)
                                {
                                    //ImGui::Text("X in range, Y poking in");
                                    circleCollider->GetParent()->SetPosition(circlePos.x, rectPos.y + (rectExtents.y * 0.5f + circleRadius) * (circlePos.y > rectPos.y ? 1 : -1));
                                    return true;
                                }
                                // X in range, Y not poking in
                                else
                                    return false;
                            }
                        }
                        else
                        {   // Y in range, X may or may not be poking in.
                            if (WithinYRange)
                            {
                                // Y in range, X poking in.
                                if (abs(circlePos.x - rectPos.x) < circleRadius + rectExtents.x * 0.5f)
                                {
                                    //ImGui::Text("Y in range, X poking in");
                                    circleCollider->GetParent()->SetPosition(rectPos.x + (rectExtents.x * 0.5f + circleRadius) * (circlePos.x > rectPos.x ? 1 : -1), circlePos.y);
                                    return true;
                                }
                                // Y in range, X not poking in.
                                else
                                    return false;
                            }
                            else
                            {
                                //CORNER CASE
                                circleCollider->GetParent()->SetPosition(rectCorners[ClosestCorner] + (circlePos - rectCorners[ClosestCorner]).Normalized() * circleRadius);
                                return true;
                            }
                        }
                    }
                    break;
                    // both of them need resolving.
                case 2:
                    //Vector2 Displacement;
                    break;
                default:
                    assert(false); // UNACCEPTABLE COLLISION SETTINGS.
                    break;
                }




            }
            /*
            if (angle <= rectDiagonalAngle || angle > 360 - rectDiagonalAngle) // RIGHT segment
            {

                ImGui::Text("Right segment");

                if (circlePos.x - circleRadius < rectpos.x + m_Extents.x * 0.5f)
                {
                    return true;
                }
                return false;
            }
            else if (angle >= rectDiagonalAngle && angle < 180 - rectDiagonalAngle) // TOP segment
            {
                ImGui::Text("Top segment");

                if (circlePos.y - circleRadius < rectpos.y + m_Extents.y * 0.5f)
                {
                    return true;
                }
                return false;
            }
            else if (angle >= 180 - rectDiagonalAngle && angle < 180 + rectDiagonalAngle) // LEFT segment
            {
                ImGui::Text("Left segment");

                if (circlePos.x + circleRadius > rectpos.y - m_Extents.y * 0.5f)
                {
                    return true;
                }
                return false;
            }
            else // BOTTOM segment
            {
                ImGui::Text("Bottom segment");

                if (circlePos.y + circleRadius > rectpos.y - m_Extents.y * 0.5f)
                {
                    return true;
                }
                return false;
            }
            */
        }

        return false;
    }
}