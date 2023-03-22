#include "Framework.h"
#include "RectangleCollider.h"

namespace fw {

    RectangleCollider::RectangleCollider(GameObject* _pparent, Vector2 _extents, Vector2 _relativePos, bool _resolveCollisions):
        Collider(_pparent)
    {
        m_RelativePosition = _relativePos;
        m_Extents = _extents;
        m_ResolveCollisions = _resolveCollisions;
        m_rectOuterCircleRadius = (m_Extents * 0.5f).Magnitude();
        m_rectInnerCircleRadius = min(m_Extents.x,m_Extents.y)*0.5f;

        m_pDebugMesh = new Mesh();

        m_pDebugMesh->MakeRect(Vector2(0.5f, 0.5f), _extents.x, _extents.y, false);

        Vector2 rectPos = GetRelativePosition();

    }

    RectangleCollider::RectangleCollider(fw::GameObject* _pparent, Vector2 _extents, Vector2 _relativePos, Camera* _pcamera, ShaderProgram* _pdebugshader, bool _resolveCollisions) :
        Collider(_pparent, _pcamera, _pdebugshader)
    {
        m_Extents = _extents;
        m_RelativePosition = _relativePos;
        m_ResolveCollisions = _resolveCollisions;
        m_rectOuterCircleRadius = (m_Extents * 0.5f).Magnitude();
        m_rectInnerCircleRadius = min(m_Extents.x, m_Extents.y) * 0.5f;

        m_pDebugMesh = new Mesh();
        m_pDebugMesh->MakeRect(Vector2(0.5f, 0.5f), _extents.x, _extents.y, false);

        Vector2 rectPos = GetRelativePosition();
    }

    RectangleCollider::RectangleCollider(): Collider(), m_Extents(), m_rectOuterCircleRadius(0), m_rectInnerCircleRadius(0)
    {
    }

    RectangleCollider::~RectangleCollider()
    {
        delete m_pDebugMesh;
    }

    Vector2 RectangleCollider::GetExtents()
    {
        return m_Extents;
    }

    void RectangleCollider::SetExtents(Vector2 _e)
    {
        m_Extents = _e;
    }

    k_ColliderType RectangleCollider::GetType()
    {
        return k_ColliderType::Rectangle;
    }

    bool RectangleCollider::CheckCollision(Collider* _pOtherCollider)
    {
        if (!m_Enabled)
            return false;

        bool overlap = false;
        if (_pOtherCollider->GetType() == k_ColliderType::Circle)
        {
            unsigned char ULDRflag = 0;
            RectangleCollider* rectCollider = this;
            CircleCollider* circleCollider = static_cast<CircleCollider*>(_pOtherCollider);

            Vector2 rectPos = rectCollider->GetRelativePosition();
            Vector2 rectExtents = rectCollider->m_Extents;

            Vector2 rectCorners[4] = {
                 rectPos + m_Extents * 0.5f,
                 rectPos + Vector2(m_Extents.x, -m_Extents.y) * 0.5f,
                 rectPos - m_Extents * 0.5f,
                 rectPos + Vector2(-m_Extents.x, m_Extents.y) * 0.5f,
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

                if (CornerSqrDistances[ClosestCorner] < circleRadius*circleRadius || abs(PosDiff.x) < circleRadius + rectExtents.x * 0.5f || abs(PosDiff.y) < circleRadius + rectExtents.y * 0.5f)
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
                                    circleCollider->GetParent()->SetPosition(circlePos.x, rectPos.y + (rectExtents.y * 0.5f + circleRadius)* (circlePos.y > rectPos.y ? 1 : -1));
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
                                circleCollider->GetParent()->SetPosition(rectCorners[ClosestCorner] + (circlePos - rectCorners[ClosestCorner] ).Normalized() * circleRadius);
                                return true;
                            }
                        }
                    }
                    break;
                    // both of them need resolving.
                case 2:

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
        if (_pOtherCollider->GetType() == k_ColliderType::Rectangle)
        {
            // order is TOP Y, LEFT X, BOTTOM Y, RIGHT X
            Vector2 ThisPos = GetRelativePosition();
            // order is BOTTOM Y, RIGHT X, TOP Y, LEFT X 
            Vector2 ThatPos = _pOtherCollider->GetRelativePosition();
            Vector2 differenceInPos = ThatPos - ThisPos;

            Vector2 ThatExtents = static_cast<RectangleCollider*>(_pOtherCollider)->GetExtents();
            Vector4 ThisData(  ThisPos.y + m_Extents.y * 0.5f,   ThisPos.x - m_Extents.x * 0.5f,   ThisPos.y - m_Extents.y * 0.5f,   ThisPos.x + m_Extents.x * 0.5f);
            Vector4 ThatData(ThatPos.y - ThatExtents.y * 0.5f, ThatPos.x + ThatExtents.x * 0.5f, ThatPos.y + ThatExtents.y * 0.5f, ThatPos.x - ThatExtents.x * 0.5f);
            Vector4 OverlapValues = ThisData - ThatData;

            overlap = (signbit(OverlapValues.x) ^ signbit(OverlapValues.z)) && (signbit(OverlapValues.y) ^ signbit(OverlapValues.w));
            unsigned char WhoseCollisionToResolve = GetResolveCollisions() + _pOtherCollider->GetResolveCollisions();

            char direction = 0; //  1 = up, 2 = left, 3 = down, 4 = right

            {
                //float scaledposX = differenceInPos.x * (ThatExtents.y / ThatExtents.x);
                //float scaledposY = differenceInPos.y * (ThatExtents.x / ThatExtents.y);
                //
                //float x = abs(differenceInPos.x);
                //float y = abs(differenceInPos.y);


                //if   (x > y) 
                //{
                //    direction = differenceInPos.x > 0 ? 2 : 4; 
                //}
                //else 
                //{
                //    direction = differenceInPos.y > 0 ? 3 : 1;
                //}
            }

            {
                float values[4] = { abs(OverlapValues.x) ,abs(OverlapValues.y), abs(OverlapValues.z),  abs(OverlapValues.w) };

                int SmallestIndex = 0;

                for (int i = 0; i < 4; i++)
                {
                    if (values[i] < values[SmallestIndex])
                        SmallestIndex = i;
                }

                direction = SmallestIndex+1;
            }


            overlap ?
                ImGui::Text("Yep"):
                ImGui::Text("Nop");

            switch (WhoseCollisionToResolve)
            {
            case 0:
                return overlap;
                break;

            case 1:

                if (overlap) {
                    if (GetResolveCollisions()) // other collider is static
                    {
                        switch (direction)
                        {
                        case 1:
                            GetParent()->SetPosition(GetParent()->GetPosition().x, _pOtherCollider->GetParent()->GetPosition().y + (m_Extents.y + ThatExtents.y) * .5f);
                            break;
                        case 2:
                            GetParent()->SetPosition(_pOtherCollider->GetParent()->GetPosition().x - (m_Extents.x + ThatExtents.x) * .5f, GetParent()->GetPosition().y);
                            break;
                        case 3:
                            GetParent()->SetPosition(GetParent()->GetPosition().x, _pOtherCollider->GetParent()->GetPosition().y - (m_Extents.y + ThatExtents.y) * .5f);
                            break;
                        case 4:
                            GetParent()->SetPosition(_pOtherCollider->GetParent()->GetPosition().x + (m_Extents.x + ThatExtents.x) * .5f, GetParent()->GetPosition().y);
                            break;
                        default:
                            assert(false);//COUNLDN'T DETECT ENTRY POINT
                            break;
                        }
                    }
                    else
                    {
                        switch (direction)
                        {
                        case 1:
                            _pOtherCollider->GetParent()->SetPosition(_pOtherCollider->GetParent()->GetPosition().x, GetParent()->GetPosition().y + (m_Extents.y + ThatExtents.y) * .5f);
                            break;
                        case 2:
                            _pOtherCollider->GetParent()->SetPosition(GetParent()->GetPosition().x - (m_Extents.x + ThatExtents.x) * .5f, _pOtherCollider->GetParent()->GetPosition().y);
                            break;
                        case 3:
                            _pOtherCollider->GetParent()->SetPosition(_pOtherCollider->GetParent()->GetPosition().x, GetParent()->GetPosition().y - (m_Extents.y + ThatExtents.y) * .5f);
                            break;
                        case 4:
                            _pOtherCollider->GetParent()->SetPosition(GetParent()->GetPosition().x + (m_Extents.x + ThatExtents.x) * .5f, _pOtherCollider->GetParent()->GetPosition().y);
                            break;
                        default:
                            //assert(false);//COUNLDN'T DETECT ENTRY POINT
                            break;
                        }
                    }
                }
                return overlap;
                break;

            case 2:                    // both rects are dynamic
               
                if (overlap)
                {
                    ImGui::Text("%d", direction);
                    float ThisArea = m_Extents.x + m_Extents.y;
                    float ThatArea = ThatExtents.x + ThatExtents.y;
                    float AreaFraction = ThisArea / (ThisArea + ThatArea);
                    Vector2 Displacement;
                    switch (direction)
                    {
                    case 1:
                        Displacement = Vector2(0,OverlapValues.x);
                        break;
                    case 2:
                        Displacement = Vector2(OverlapValues.y,0);
                        break;
                    case 3:
                        Displacement = Vector2(0, OverlapValues.z);
                        break;
                    case 4:
                        Displacement = Vector2(OverlapValues.w, 0);
                        break;
                    default:
                        assert(false); //COUNLDN'T DETECT ENTRY POINT
                        break;
                    }

                    _pOtherCollider->GetParent()->SetPosition(_pOtherCollider->GetParent()->GetPosition() + Displacement * AreaFraction);
                    GetParent()->SetPosition(GetParent()->GetPosition() - Displacement * (1-AreaFraction));
                }

                return overlap;
                break;

            default:
                /// UNEXPECTED INT
                //assert(false);
                break;
            }

            return (overlap);
        }
        return false;
    }
}