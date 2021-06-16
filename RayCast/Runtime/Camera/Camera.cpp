#include "Camera.h"

#include <iostream>

Camera::Camera(glm::vec3 position, float aspectRatio, glm::vec3 up, glm::vec3 front, float fov, float nearplane, float farplane, glm::vec3 direction)
    : m_Position(position),
    m_aspectRatio(aspectRatio),
    m_fov(fov),
    m_Up(up),
    m_nearPlane(nearplane),
    m_farPlane(farplane),
    m_cameraUp(glm::vec3(0.0f)),
    m_Front(front)
{
    this->m_viewMatrix = glm::mat4(1.0f);
    m_MovementSpeed = 3.0f;

    m_pitch = 0.f;
    m_yaw = -90.f;
    m_roll = 0.f;

    m_viewMatrix = glm::mat4(1.f);
    m_viewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);

    m_projectionMatrix = glm::mat4(1.f);
    m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);

    m_Right = glm::normalize(glm::cross(m_Front, m_Up));


}

Camera::~Camera()
{
}

void Camera::OnUpdate(float DeltaTime,GLFWwindow* window)
{
    Recalculate();
}

void Camera::Move(EditorCameraDirection direction, float DeltaTime)
{
    switch (direction)
    {
    case EditorCameraDirection::FORWARD:
        m_Position += m_MovementSpeed * m_Front * DeltaTime;
        break;
    case EditorCameraDirection::BACK:
        m_Position -= m_MovementSpeed * m_Front * DeltaTime;
        break;
    case EditorCameraDirection::UP:
        m_Position.y +=  m_MovementSpeed * DeltaTime;
        break;
    case EditorCameraDirection::DOWN:
        m_Position.y -= m_MovementSpeed * DeltaTime;
        break;
    case EditorCameraDirection::LEFT:
        m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * m_MovementSpeed * DeltaTime;
        break;
    case EditorCameraDirection::RIGHT:
        m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * m_MovementSpeed * DeltaTime;
        break;
    default:
        break;
    }
    Recalculate();

}

void Camera::Rotate(EditorCameraRotationDirection direction, float DeltaTime)
{
    switch (direction)
    {
    case EditorCameraRotationDirection::LEFT:
        m_yaw += -35.f * DeltaTime;
        break;
    case EditorCameraRotationDirection::RIGHT:
        m_yaw += 35.f * DeltaTime;
        break;
    case EditorCameraRotationDirection::UP:
        m_pitch += 35.f * DeltaTime;
        break;
    case EditorCameraRotationDirection::DOWN:
        m_pitch += -35.f * DeltaTime;
        break;
    default:
        break;
    }

    glm::vec3 l_direction;
    l_direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    l_direction.y = sin(glm::radians(m_pitch));
    l_direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_Front = glm::normalize(l_direction);


    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    Recalculate();
}

void Camera::Zoom(float ScaleFactor)
{
    m_fov -= ScaleFactor;
    if (m_fov < 1.0f)
        m_fov = 1.0f;
    if (m_fov > 90.0f)
        m_fov = 90.0f;

    Recalculate();  
}

void Camera::Recalculate()
{
    m_viewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
}

const glm::vec3& Camera::GetPosition()
{
    return m_Position;
}

void Camera::CalculatePosition(float horizontalDistance, float verticalDistance)
{

}


const glm::mat4& Camera::GetViewProjectionMatrix()
{
    return m_viewMatrix * m_projectionMatrix;
}

const glm::mat4& Camera::GetViewMatrix() const
{
    return m_viewMatrix;
}

const glm::mat4& Camera::GetProjectionMatrix() const
{
    return m_projectionMatrix;
}

const float& Camera::GetFov()
{
    return m_fov;
}

