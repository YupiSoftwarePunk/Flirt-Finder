using System.ComponentModel.DataAnnotations;
using System.ComponentModel.DataAnnotations.Schema;

namespace Server.Models
{
    [Table("users")]
    public class User
    {
        [Key]
        [Column("id")]
        public int Id { get; set; }

        [Column("name")]
        public string Username { get; set; }

        [Column("hobbies")]
        public string Bio { get; set; }

        [Column("gender")]
        public string Gender { get; set; }

        [Column("age")]
        public DateTime? BirthDate { get; set; }

        //[Column("photo_path")]
        //public string PhotoUrl { get; set; }

        [Column("password")]
        public string PasswordHash { get; set; }

        [Column("login")]
        public string Login { get; set; }

        [Column("city")]
        public string City { get; set; }


        public ICollection<Photo> Photos { get; set; }
    }
}
