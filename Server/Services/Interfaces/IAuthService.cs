using Microsoft.AspNetCore.Mvc;

namespace Server.Services.Interfaces
{
    public interface IAuthService
    {
        public async Task<IActionResult> Register(RegisterDto dto);

        public async Task<IActionResult> Login(LoginDto dto);
    }
}
