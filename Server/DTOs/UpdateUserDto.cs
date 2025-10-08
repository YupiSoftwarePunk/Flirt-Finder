namespace Server.DTOs
{
    public class UpdateUserDto
    {
        public string FullName { get; set; }
        public string Bio { get; set; }
        public string Gender { get; set; }
        public DateTime? BirthDate { get; set; }
        public string PhotoUrl { get; set; }
    }
}
