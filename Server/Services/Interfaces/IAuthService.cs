using Microsoft.AspNetCore.Mvc;
using Server.DTOs;

namespace Server.Services.Interfaces
{
    public interface IAuthService
    {
        Task<AuthResultDto> Register(RegisterDto dto);

        Task<string> Login(LoginDto dto);
    }
}
