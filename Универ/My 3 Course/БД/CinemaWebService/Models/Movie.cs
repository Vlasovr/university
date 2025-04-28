using MongoDB.Bson;
using MongoDB.Bson.Serialization.Attributes;

namespace CinemaWebService.Models
{
    public class Movie
    {
        [BsonId] // Указывает, что это поле является идентификатором документа
        [BsonRepresentation(BsonType.ObjectId)] // Позволяет использовать строковое представление ObjectId
        public string? Id { get; set; } // Свойство для хранения _id из MongoDB опционально для него, для posgres оно должно неопциональным
        public int MovieId { get; set; } // PK
        public string Name { get; set; }
        public int Duration { get; set; }
        public string Genre { get; set; }
        
        [BsonIgnore]
        public ICollection<Review> Reviews { get; set; } = new HashSet<Review>();
        [BsonIgnore]
        public ICollection<SessionMovie> SessionMovies { get; set; } = new HashSet<SessionMovie>();
    }
}