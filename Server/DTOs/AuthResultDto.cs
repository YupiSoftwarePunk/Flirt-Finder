namespace Server.DTOs
{
    public class AuthResultDto
    {
        public bool Success { get; set; }
        public string Message { get; set; }
        public string Token { get; set; }
    }
}
